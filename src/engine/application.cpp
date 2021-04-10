#include <cstdlib>
#include <exception>

#include "application.hpp"
#include "broadcast.hpp"
#include "gfx/renderer.hpp"
#include "gfx/window.hpp"
#include "layer_stack.hpp"
#include "systems/render_system.hpp"
#include "util/io_log_sink.hpp"
#include "util/log.hpp"

namespace fge
{

application::application()
    : application_broadcast(new broadcast),
      renderer(new gfx::renderer(application_broadcast.get()))
{
  start_logger<io_log_sink>(log_level::debug, log_mode::sync);
}

void application::init(int /*argc*/, char ** /*argv*/)
{
  try
  {
    init_application();
    renderer->init();

    application_layers.init();
  }
  catch (std::exception &e)
  {
    error("application",
          "Unhandled exception while init application: {}",
          e.what());
  }
}

int application::run()
{
  delta_timer.start();
  try
  {
    main_loop();
  }
  catch (std::exception &e)
  {
    error("application",
          "Unhandled exception while executing main loop: {}",
          e.what());
  }
  return EXIT_SUCCESS;
}

void application::main_loop()
{
  while (!close_application)
  {
    delta_timer.update();
    renderer->start_frame();
    application_layers.update(delta_timer.get());
    renderer->render_frame();
  }

  terminate();
}

void application::terminate()
{
  application_layers.terminate();
  renderer->terminate();
  terminate_logger();
}

void application::init_application()
{
  application_broadcast->subscribe<gfx::window_close_event>(
      [&](const gfx::window_close_event &) -> bool {
        close();
        return false;
      });
}

gfx::renderer *application::get_renderer() { return renderer.get(); }

broadcast *application::get_application_broadcast() const
{
  return application_broadcast.get();
}

void application::close() { close_application = true; }

} // namespace fge

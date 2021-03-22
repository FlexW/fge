#include <cstdlib>

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
  init_application();
  renderer->init();

  application_layers.init();
}

int application::run()
{
  main_loop();
  return EXIT_SUCCESS;
}

void application::main_loop()
{
  while (!close_application)
  {
    application_layers.update();
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
        close_application = true;
        return false;
      });
}

} // namespace fge

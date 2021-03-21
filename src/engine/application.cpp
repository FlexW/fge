#include <cstdlib>

#include "application.hpp"
#include "gfx/renderer.hpp"

namespace fge
{

application::application() : renderer(new gfx::renderer) {}

void application::init(int /*argc*/, char ** /*argv*/) { renderer->init(); }

int application::run()
{
  main_loop();
  return EXIT_SUCCESS;
}

void application::main_loop()
{
  while (true)
  {
    renderer->render_frame();
  }
}

} // namespace fge

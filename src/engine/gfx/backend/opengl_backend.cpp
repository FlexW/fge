#include <memory>

#include "gfx/graphic_backend_context.hpp"
#include "glfw_window.hpp"

namespace fge::gfx::gl
{

std::unique_ptr<window> create_window(broadcast *application_broadcast,
                                      const window_create_info &create_info)
{
  return std::make_unique<glfw::glfw_window>(application_broadcast,
                                             create_info);
}

graphic_backend_context load_opengl_backend_context()
{
  graphic_backend_context context{};

  context.create_window = create_window;

  return context;
}

} // namespace fge::gfx::gl

#include <memory>

#include "graphic_backend.hpp"
#include "renderer.hpp"

namespace fge::gfx
{

renderer::renderer(broadcast *application_broadcast)
    : application_broadcast(application_broadcast)
{
}

void renderer::init()
{
  graphic_backend::init();

  window_create_info win_create_info{};

  renderer_window =
      graphic_backend::create_window(application_broadcast, win_create_info);
}

void renderer::render_frame() { renderer_window->flush(); }

window *renderer::get_renderer_window() const { return renderer_window.get(); }

void renderer::terminate() { renderer_window->terminate(); }

} // namespace fge::gfx

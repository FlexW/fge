#include <memory>

#include "gfx/command_bucket.hpp"
#include "gfx/command_packet.hpp"
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
  gfx::init_buckets();

  window_create_info win_create_info{};

  renderer_window =
      graphic_backend::create_window(application_broadcast, win_create_info);
}

void renderer::render_frame()
{
  gfx::graphic_backend::render_frame();
  renderer_window->flush();
}

window *renderer::get_renderer_window() const { return renderer_window.get(); }

command_bucket *renderer::create_command_bucket()
{
  return command_bucket::create();
}

void renderer::terminate() { renderer_window->terminate(); }

void renderer::start_frame() { gfx::command_bucket::start_frame(); }

} // namespace fge::gfx

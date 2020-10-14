#include "render_view.hpp"
#include "application.hpp"
#include "graphic/graphic_manager.hpp"
#include "util/assert.hpp"

namespace Fge
{

RenderView::RenderView(Target   target,
                       uint32_t width,
                       uint32_t height,
                       uint32_t samples)
    : target(target),
      width(width),
      height(height),
      samples(samples)
{
  // FIXME: Can just handle backbuffer atm
  FGE_ASSERT(target == Target::Backbuffer);
  FGE_ASSERT(samples == 0);
}

void RenderView::render(const glm::mat4 &projection_mat,
                        const Camera &   camera,
                        uint32_t         width,
                        uint32_t         height,
                        uint32_t /*samples*/)
{
  auto app             = Application::get_instance();
  auto graphic_manager = app->get_graphic_manager();
  auto renderer        = graphic_manager->get_renderer();
  auto render_path     = graphic_manager->get_render_path();

  // Check if target needs recreated
  // Bind target

  renderer->set_viewport(0, 0, width, height);
  render_path->render(projection_mat, camera);
}

} // namespace Fge

#include "forward_render_path.hpp"
#include "application.hpp"

namespace Fge
{

void ForwardRenderPath::render(const glm::mat4 &projection_mat,
                               const Camera &   camera,
                               uint32_t         width,
                               uint32_t         height)
{
  auto app             = Application::get_instance();
  auto graphic_manager = app->get_graphic_manager();
  auto renderer        = graphic_manager->get_renderer();

  renderer->clear_color();
  renderer->clear_depth();
  renderer->set_viewport(0, 0, width, height);

  const auto &renderables = renderer->get_renderables();

  const auto view_mat = camera.get_view_matrix();
  for (auto renderable : renderables)
  {
    auto material = renderable->get_material();
    material->set_uniform("projection_mat", projection_mat);
    material->set_uniform("view_mat", view_mat);

    renderer->draw(*renderable->get_vertex_array(),
                   *renderable->get_index_buffer(),
                   *material);
  }
}

} // namespace Fge

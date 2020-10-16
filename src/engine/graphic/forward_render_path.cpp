#include <fmt/core.h>

#include "application.hpp"
#include "forward_render_path.hpp"
#include "graphic/material.hpp"
#include "graphic/point_light.hpp"
#include "log/log.hpp"
#include <vector>

// This value must be the same like in the corresponding shaders
#define MAX_POINT_LIGHTS_COUNT 5

namespace Fge
{

void ForwardRenderPath::set_lightning_uniforms(
    Material &                                      material,
    const std::vector<std::shared_ptr<PointLight>> &point_lights)
{
  int32_t point_light_count = 0;
  for (uint32_t i = 0; i < point_lights.size() && i < MAX_POINT_LIGHTS_COUNT;
       ++i)
  {
    material.set_uniform(
        fmt::format("point_lights[{}].position", static_cast<int32_t>(i)),
        point_lights[i]->position);

    ++point_light_count;
  }

  material.set_uniform("point_light_count", point_light_count);
}

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

  const auto &renderables  = renderer->get_renderables();
  const auto &point_lights = renderer->get_point_lights();

  const auto view_mat = camera.get_view_matrix();
  for (auto renderable : renderables)
  {
    auto material = renderable->get_material();
    material->set_uniform("projection_mat", projection_mat);
    material->set_uniform("view_mat", view_mat);
    set_lightning_uniforms(*material, point_lights);

    if (renderable->get_index_buffer())
    {
      renderer->draw(*renderable->get_vertex_array(),
                     *renderable->get_index_buffer(),
                     *material,
                     renderable->get_draw_mode());
    }
    else
    {
      renderer->draw(*renderable->get_vertex_array(),
                     *material,
                     renderable->get_draw_mode());
    }
  }
}

} // namespace Fge

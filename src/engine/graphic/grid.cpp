#include "grid.hpp"
#include "application.hpp"
#include "graphic/unlit_material.hpp"
#include "graphic/vertex_array.hpp"
#include "graphic/vertices.hpp"
#include "util/assert.hpp"

namespace Fge
{

Grid::Grid(int32_t size)
{
  FGE_ASSERT(0 < size && size % 2 == 0);

  const int32_t size_half = size / 2;

  std::vector<VertexP> vertices;

  for (int32_t i = -size_half; i <= size_half; ++i)
  {
    vertices.push_back({glm::vec3(i, 0.0f, size_half)});
    vertices.push_back({glm::vec3(i, 0.0f, -size_half)});

    vertices.push_back({glm::vec3(size_half, 0.0f, i)});
    vertices.push_back({glm::vec3(-size_half, 0.0f, i)});
  }

  auto app      = Application::get_instance();
  auto renderer = app->get_graphic_manager()->get_renderer();

  auto vertex_buffer = renderer->create_vertex_buffer_p(vertices);

  vertex_array = renderer->create_vertex_array();
  vertex_array->add_buffer(vertex_buffer);

  material = std::make_shared<UnlitMaterial>();
  material->set_world_matrix(glm::mat4(1.0f));
  material->set_color(glm::vec3(0.35f));
}

std::shared_ptr<RenderInfo> Grid::create_render_info()
{

  auto render_info = std::make_shared<RenderInfo>(vertex_array, material);
  render_info->set_draw_mode(DrawMode::LINES);

  return render_info;
}

} // namespace Fge

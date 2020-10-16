#include "unlit_material.hpp"
#include "application.hpp"
#include "graphic/mesh_material.hpp"

namespace Fge
{

UnlitMaterial::UnlitMaterial() : MeshMaterial("Unlit Material")
{
  auto app      = Application::get_instance();
  auto renderer = app->get_graphic_manager()->get_renderer();

  shader = renderer->create_shader("unlit.vert",
                                   "unlit.frag",
                                   std::vector<std::string>{});
}

void UnlitMaterial::bind(uint32_t texture_bind_point)
{
  shader->bind();
  bind_uniforms(shader, texture_bind_point);
}

void UnlitMaterial::set_skinned_mesh(bool /*value*/) {}

void UnlitMaterial::set_rigid_mesh(bool /*value*/) {}

void UnlitMaterial::set_world_matrix(const glm::mat4 &world_mat)
{
  set_uniform("world_mat", world_mat);
}

void UnlitMaterial::set_color(const glm::vec3 &color)
{
  set_uniform("color", color);
}

std::shared_ptr<Material> UnlitMaterial::clone()
{
  auto new_material = std::make_shared<UnlitMaterial>();

  new_material->set_world_matrix(glm::mat4(1.0f));

  for (const auto &uniform : uniforms)
  {
    auto new_uniform = uniform->clone();
    new_material->uniforms.push_back(new_uniform);
  }

  return new_material;
}

} // namespace Fge

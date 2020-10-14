#include "default_material.hpp"
#include "application.hpp"
#include "graphic/material.hpp"

namespace Fge
{

DefaultMaterial::DefaultMaterial() : MeshMaterial("Default Material") {}

void DefaultMaterial::bind(uint32_t texture_bind_point)
{
  if (shader_changed)
  {
    regenerate_shader();
  }

  shader->bind();
  bind_uniforms(shader, texture_bind_point);
}

void DefaultMaterial::set_skinned_mesh(bool value)
{
  if (value)
  {
    shader_defines.push_back("SKINNED");
    shader_changed = true;
  }
}

void DefaultMaterial::set_rigid_mesh(bool /*value*/) {}

void DefaultMaterial::set_world_matrix(const glm::mat4 &world_mat)
{
  set_uniform("world_mat", world_mat);
}

void DefaultMaterial::regenerate_shader()
{
  auto app      = Application::get_instance();
  auto renderer = app->get_graphic_manager()->get_renderer();

  shader = renderer->create_shader("blinn_phong.vert",
                                   "blinn_phong.frag",
                                   shader_defines);
}

std::shared_ptr<Material> DefaultMaterial::clone()
{
  auto new_material = std::make_shared<DefaultMaterial>();

  new_material->set_world_matrix(glm::mat4(1.0f));

  for (const auto &define : shader_defines)
  {
    new_material->shader_defines.push_back(define);
  }

  for (const auto &uniform : uniforms)
  {
    auto new_uniform = uniform->clone();
    new_material->uniforms.push_back(new_uniform);
  }

  return new_material;
}

} // namespace Fge

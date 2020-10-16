#include "default_material.hpp"
#include "application.hpp"
#include "graphic/material.hpp"
#include "util/assert.hpp"

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

void DefaultMaterial::set_ambient_texture(std::shared_ptr<Texture2D> tex)
{
  FGE_ASSERT(tex);
  set_uniform("in_ambient_tex", tex);

  if (!shader_defines_contains("AMBIENT_TEX"))
  {
    shader_defines.push_back("AMBIENT_TEX");
    shader_changed = true;
  }
}

void DefaultMaterial::set_diffuse_texture(std::shared_ptr<Texture2D> tex)
{
  FGE_ASSERT(tex);
  set_uniform("in_diffuse_tex", tex);

  if (!shader_defines_contains("DIFFUSE_TEX"))
  {
    shader_defines.push_back("DIFFUSE_TEX");
    shader_changed = true;
  }
}

void DefaultMaterial::set_specular_texture(std::shared_ptr<Texture2D> tex)
{
  FGE_ASSERT(tex);
  set_uniform("in_specular_tex", tex);

  if (!shader_defines_contains("SPECULAR_TEX"))
  {
    shader_defines.push_back("SPECULAR_TEX");
    shader_changed = true;
  }
}

void DefaultMaterial::regenerate_shader()
{
  auto app      = Application::get_instance();
  auto renderer = app->get_graphic_manager()->get_renderer();

  shader         = renderer->create_shader("blinn_phong.vert",
                                   "blinn_phong.frag",
                                   shader_defines);
  shader_changed = false;
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

bool DefaultMaterial::shader_defines_contains(const std::string &define)
{
  auto iter = std::find(shader_defines.begin(), shader_defines.end(), define);

  if (iter != shader_defines.end())
  {
    return true;
  }

  return false;
}

void DefaultMaterial::set_specular_power(float specular_power)
{
  set_uniform("specular_power", specular_power);
}

} // namespace Fge

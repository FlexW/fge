#include "material.hpp"

namespace Fge
{

Material::Material(const std::string &name) : name(name) {}

uint32_t Material::bind_uniforms(std::shared_ptr<Shader> shader,
                                 uint32_t texture_bind_point) const
{
  shader->bind();
  for (auto &uniform : uniforms)
  {
    texture_bind_point = uniform->set(*shader);
  }
  return texture_bind_point;
}

void Material::unbind() const {}

void Material::reload() {}
}

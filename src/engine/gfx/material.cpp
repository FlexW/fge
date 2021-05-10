#include "material.hpp"

namespace fge::gfx
{

void material::set_uniform(const std::string &name, const glm::vec3 &value)
{
  add_or_modify_uniform(vec3_uniforms, name, value);
}

void material::set_uniform(const std::string &name, const glm::mat4 &value)
{
  add_or_modify_uniform(texture_uniforms, name, value);
}

void material::set_uniform(const std::string &name, const texture_handle value)
{
  add_or_modify_uniform(texture_uniforms, name, value);
}

} // namespace fge::gfx

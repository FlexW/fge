#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "gfx/command_bucket.hpp"
#include "gfx/graphic_backend_types.hpp"

namespace fge::gfx
{

class material
{
public:
  material(std::shared_ptr<shader_program_handle> shader);

  void set_uniform(const std::string &name, const glm::vec3 &value);

  void set_uniform(const std::string &name, const glm::mat4 &value);

  void set_uniform(const std::string &name, const texture_handle value);

private:
  template <typename TValue> struct uniform
  {
    std::string name;
    TValue      value;
  };

  std::vector<uniform<texture_handle>> texture_uniforms;
  std::vector<uniform<glm::vec3>>      vec3_uniforms;

  std::shared_ptr<shader_program_handle> shader;

  template <typename TUniform, typename TUniformContainer>
  void add_or_modify_uniform(TUniformContainer &&container,
                             const std::string & name,
                             TUniform &&         value)
  {
    auto iter =
        std::find_if(container.begin(),
                     container.end(),
                     [&](const auto &uniform) { return name == uniform.name; });

    if (iter == container.end())
    {
      container.emplace_back(name, value);
      return;
    }

    *iter = {name, value};
  }
};

} // namespace fge::gfx

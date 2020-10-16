#pragma once

#include "math/math.hpp"

namespace Fge
{

struct PointLight
{
  glm::vec3 position{};
  glm::vec3 ambient_color  = glm::vec3(0.2f);
  glm::vec3 diffuse_color  = glm::vec3(0.6f);
  glm::vec3 specular_color = glm::vec3(1.0f);
};

} // namespace Fge

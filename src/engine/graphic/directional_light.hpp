#pragma once

#include "math/math.hpp"

namespace Fge
{

struct DirectionalLight
{
  glm::vec3 direction      = glm::vec3(0.33f, 0.33f, 0.33f);
  glm::vec3 ambient_color  = glm::vec3(0.6f);
  glm::vec3 diffuse_color  = glm::vec3(0.8f);
  glm::vec3 specular_color = glm::vec3(1.0f);
};

} // namespace Fge

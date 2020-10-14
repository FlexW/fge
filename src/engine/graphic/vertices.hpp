#pragma once

#include "math/math.hpp"

namespace Fge
{

struct VertexP
{
  glm::vec3 positon;
};

struct VertexPNT
{
  glm::vec3 position  = glm::vec3(0.0f);
  glm::vec3 normal    = glm::vec3(0.0f);
  glm::vec2 tex_coord = glm::vec2(0.0f);
};

struct VertexPNTBT
{
  glm::vec3 position  = glm::vec3(0.0f);
  glm::vec3 normal    = glm::vec3(0.0f);
  glm::vec3 tangent   = glm::vec3(0.0f);
  glm::vec3 bitangent = glm::vec3(0.0f);
  glm::vec2 tex_coord = glm::vec2(0.0f);
};

struct VertexPNTBBWT
{
  glm::vec3  position     = glm::vec3(0.0f);
  glm::vec3  normal       = glm::vec3(0.0f);
  glm::vec3  tangent      = glm::vec3(0.0f);
  glm::vec3  bitangent    = glm::vec3(0.0f);
  glm::ivec4 skin_bones   = glm::ivec4(0, 0, 0, 0);
  glm::vec4  skin_weights = glm::vec4(0.25f, 0.25f, 0.25f, 0.25f);
  glm::vec2  tex_coord    = glm::vec2(0.0f);
};

} // namespace Fge

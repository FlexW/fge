#pragma once

#include "material.hpp"
#include "math/math.hpp"

namespace Fge
{

class MeshMaterial : public Material
{
public:
  MeshMaterial(const std::string &name) : Material(name) {}

  virtual ~MeshMaterial() = default;

  virtual void set_skinned_mesh(bool value) = 0;

  virtual void set_rigid_mesh(bool value) = 0;

  virtual void set_world_matrix(const glm::mat4 &world_mat) = 0;

  virtual void set_bone_transforms(const std::vector<glm::mat4> &bones) = 0;
};

} // namespace Fge

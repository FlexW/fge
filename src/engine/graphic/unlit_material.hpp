#pragma once

#include "graphic/mesh_material.hpp"
namespace Fge
{

class UnlitMaterial : public MeshMaterial
{
public:
  UnlitMaterial();

  void bind(uint32_t texture_bind_point = 0) override;

  void set_skinned_mesh(bool value) override;

  void set_rigid_mesh(bool value) override;

  void set_world_matrix(const glm::mat4 &world_mat) override;

  void set_bone_transforms(const std::vector<glm::mat4> &bones) override;

  void set_color(const glm::vec3 &color);

  std::shared_ptr<Material> clone() override;

private:
  std::shared_ptr<Shader> shader{};
};

} // namespace Fge

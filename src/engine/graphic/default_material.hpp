#pragma once

#include "material.hpp"
#include "mesh_material.hpp"
#include "shader.hpp"

namespace Fge
{

class DefaultMaterial : public MeshMaterial
{
public:
  DefaultMaterial();

  void bind(uint32_t texture_bind_point = 0) override;

  void set_skinned_mesh(bool value) override;

  void set_rigid_mesh(bool value) override;

  void set_world_matrix(const glm::mat4 &world_mat) override;

  std::shared_ptr<Material> clone() override;

private:
  std::shared_ptr<Shader> shader;

  std::vector<std::string> shader_defines;

  bool shader_changed = true;

  void regenerate_shader();
};

} // namespace Fge

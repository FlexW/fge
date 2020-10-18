#pragma once

#include "graphic/skeleton.hpp"
#include "mesh_base.hpp"

namespace Fge
{

class SkinnedSubMesh : public SubMeshBase<VertexPNTBBWT>
{
public:
  SkinnedSubMesh(const std::string &                         name,
                 std::shared_ptr<std::vector<VertexPNTBBWT>> vertices,
                 std::shared_ptr<std::vector<uint32_t>>      indices,
                 std::shared_ptr<MeshMaterial>               material);

  SkinnedSubMesh(const std::string &                         name,
                 std::shared_ptr<std::vector<VertexPNTBBWT>> vertices,
                 std::shared_ptr<std::vector<uint32_t>>      indices,
                 std::shared_ptr<MeshMaterial>               material,
                 std::shared_ptr<VertexArray>                vertex_array,
                 std::shared_ptr<IndexBuffer>                index_buffer);
};

class SkinnedMesh : public MeshBase<VertexPNTBBWT>
{
public:
  SkinnedMesh(const std::vector<std::shared_ptr<SkinnedSubMesh>> &sub_meshes,
              const Skeleton &                                    skeleton);

  void play_animation(const std::string &name);

  void play_animation_endless(const std::string &name);

  void stop_current_animation();

  const std::vector<glm::mat4> &compute_bone_transforms();

  const Skeleton &get_skeleton() { return skeleton; }

protected:
  Skeleton skeleton;
};

} // namespace Fge

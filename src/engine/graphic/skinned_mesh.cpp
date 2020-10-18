#include "skinned_mesh.hpp"
#include "graphic/skeleton.hpp"
#include "graphic/vertices.hpp"
#include "util/time.hpp"

namespace Fge
{

SkinnedSubMesh::SkinnedSubMesh(
    const std::string &                         name,
    std::shared_ptr<std::vector<VertexPNTBBWT>> vertices,
    std::shared_ptr<std::vector<uint32_t>>      indices,
    std::shared_ptr<MeshMaterial>               material)
    : SubMeshBase<VertexPNTBBWT>(name, vertices, indices, material)
{
  material->set_skinned_mesh(true);
}

SkinnedSubMesh::SkinnedSubMesh(
    const std::string &                         name,
    std::shared_ptr<std::vector<VertexPNTBBWT>> vertices,
    std::shared_ptr<std::vector<uint32_t>>      indices,
    std::shared_ptr<MeshMaterial>               material,
    std::shared_ptr<VertexArray>                vertex_array,
    std::shared_ptr<IndexBuffer>                index_buffer)
    : SubMeshBase<VertexPNTBBWT>(name,
                                 vertices,
                                 indices,
                                 material,
                                 vertex_array,
                                 index_buffer)
{
}

SkinnedMesh::SkinnedMesh(
    const std::vector<std::shared_ptr<SkinnedSubMesh>> &sub_meshes,
    const Skeleton &                                    skeleton)
    : skeleton(skeleton)
{
  for (auto sub_mesh : sub_meshes)
  {
    add_sub_mesh(sub_mesh);
  }
}

void SkinnedMesh::play_animation(const std::string &name)
{
  double current_time = static_cast<double>(get_current_time_millis());
  current_time /= 1000;
  skeleton.play_animation(name, current_time);
}

void SkinnedMesh::play_animation_endless(const std::string &name)
{
  double current_time = static_cast<double>(get_current_time_millis());
  current_time /= 1000;
  skeleton.play_animation_endless(name, current_time);
}

void SkinnedMesh::stop_current_animation()
{
  skeleton.stop_current_animation();
}

const std::vector<glm::mat4> &SkinnedMesh::compute_bone_transforms()
{
  double current_time = static_cast<double>(get_current_time_millis());
  current_time /= 1000;
  return skeleton.compute_bone_transforms(current_time);
}

} // namespace Fge

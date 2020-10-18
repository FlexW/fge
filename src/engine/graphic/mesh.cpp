#include "mesh.hpp"
#include "application.hpp"
#include "graphic/vertices.hpp"
#include "vertex_buffer_layout.hpp"

namespace Fge
{

SubMesh::SubMesh(const std::string &                       name,
                 std::shared_ptr<std::vector<VertexPNTBT>> vertices,
                 std::shared_ptr<std::vector<uint32_t>>    indices,
                 std::shared_ptr<MeshMaterial>             material)
    : SubMeshBase<VertexPNTBT>(name, vertices, indices, material)
{
}

SubMesh::SubMesh(const std::string &                       name,
                 std::shared_ptr<std::vector<VertexPNTBT>> vertices,
                 std::shared_ptr<std::vector<uint32_t>>    indices,
                 std::shared_ptr<MeshMaterial>             material,
                 std::shared_ptr<VertexArray>              vertex_array,
                 std::shared_ptr<IndexBuffer>              index_buffer)
    : SubMeshBase<VertexPNTBT>(name,
                               vertices,
                               indices,
                               material,
                               vertex_array,
                               index_buffer)
{
}

Mesh::Mesh(const std::vector<std::shared_ptr<SubMesh>> &sub_meshes)
{
  for (auto sub_mesh : sub_meshes)
  {
    add_sub_mesh(sub_mesh);
  }
}

} // namespace Fge

#pragma once

#include "mesh_base.hpp"

namespace Fge
{

class SubMesh : public SubMeshBase<VertexPNTBT>
{
public:
  SubMesh(const std::string &                       name,
          std::shared_ptr<std::vector<VertexPNTBT>> vertices,
          std::shared_ptr<std::vector<uint32_t>>    indices,
          std::shared_ptr<MeshMaterial>             material);

  SubMesh(const std::string &                       name,
          std::shared_ptr<std::vector<VertexPNTBT>> vertices,
          std::shared_ptr<std::vector<uint32_t>>    indices,
          std::shared_ptr<MeshMaterial>             material,
          std::shared_ptr<VertexArray>              vertex_array,
          std::shared_ptr<IndexBuffer>              index_buffer);
};

class Mesh : public MeshBase<VertexPNTBT>
{
public:
  Mesh(const std::vector<std::shared_ptr<SubMesh>> &sub_meshes);
};

} // namespace Fge

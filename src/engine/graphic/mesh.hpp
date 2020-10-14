#pragma once

#include "index_buffer.hpp"
#include "mesh_material.hpp"
#include "vertex_array.hpp"
#include "vertices.hpp"

namespace Fge
{

class SubMesh
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

  const std::string &get_name() const { return name; }

  std::shared_ptr<VertexArray> get_vertex_array() { return vertex_array; }

  std::shared_ptr<IndexBuffer> get_index_buffer() { return index_buffer; }

  std::shared_ptr<MeshMaterial> get_material() { return material; }

  std::shared_ptr<std::vector<VertexPNTBT>> get_vertices() { return vertices; }

  std::shared_ptr<std::vector<uint32_t>> get_indices() { return indices; }

private:
  std::string name;

  std::shared_ptr<std::vector<VertexPNTBT>> vertices{};
  std::shared_ptr<std::vector<uint32_t>>    indices{};

  std::shared_ptr<VertexArray> vertex_array{};
  std::shared_ptr<IndexBuffer> index_buffer{};

  std::shared_ptr<MeshMaterial> material{};
};

class Mesh
{
public:
  Mesh(const std::vector<std::shared_ptr<SubMesh>> &sub_meshes);

  std::vector<std::shared_ptr<SubMesh>> &get_sub_meshes() { return sub_meshes; }

private:
  std::vector<std::shared_ptr<SubMesh>> sub_meshes{};
};

} // namespace Fge

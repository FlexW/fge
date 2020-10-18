#pragma once

#include "application.hpp"
#include "index_buffer.hpp"
#include "mesh_material.hpp"
#include "vertex_array.hpp"
#include "vertices.hpp"

namespace Fge
{

template <typename TVertex> class SubMeshBase
{
public:
  SubMeshBase(const std::string &                    name,
              std::shared_ptr<std::vector<TVertex>>  vertices,
              std::shared_ptr<std::vector<uint32_t>> indices,
              std::shared_ptr<MeshMaterial>          material)
      : name(name),
        vertices(vertices),
        indices(indices),
        material(material)
  {
    auto app             = Application::get_instance();
    auto graphic_manager = app->get_graphic_manager();
    auto renderer        = graphic_manager->get_renderer();

    vertex_array = renderer->create_vertex_array();

    auto vertex_buffer = renderer->create_vertex_buffer(*vertices);

    index_buffer = renderer->create_index_buffer(*indices);

    vertex_array->add_buffer(vertex_buffer);
    vertex_array->add_buffer(index_buffer);
  }

  SubMeshBase(const std::string &                    name,
              std::shared_ptr<std::vector<TVertex>>  vertices,
              std::shared_ptr<std::vector<uint32_t>> indices,
              std::shared_ptr<MeshMaterial>          material,
              std::shared_ptr<VertexArray>           vertex_array,
              std::shared_ptr<IndexBuffer>           index_buffer)
      : name(name),
        vertices(vertices),
        indices(indices),
        vertex_array(vertex_array),
        index_buffer(index_buffer),
        material(material)
  {
  }

  virtual ~SubMeshBase() = default;

  const std::string &get_name() const { return name; }

  std::shared_ptr<VertexArray> get_vertex_array() { return vertex_array; }

  std::shared_ptr<IndexBuffer> get_index_buffer() { return index_buffer; }

  std::shared_ptr<MeshMaterial> get_material() { return material; }

  std::shared_ptr<std::vector<TVertex>> get_vertices() { return vertices; }

  std::shared_ptr<std::vector<uint32_t>> get_indices() { return indices; }

private:
  std::string name;

  std::shared_ptr<std::vector<TVertex>>     vertices{};
  std::shared_ptr<std::vector<uint32_t>>    indices{};

  std::shared_ptr<VertexArray> vertex_array{};
  std::shared_ptr<IndexBuffer> index_buffer{};

  std::shared_ptr<MeshMaterial> material{};
};

template <typename TVertex> class MeshBase
{
public:
  MeshBase() = default;

  MeshBase(const std::vector<std::shared_ptr<SubMeshBase<TVertex>>> &sub_meshes)
      : sub_meshes(sub_meshes)
  {
  }

  virtual ~MeshBase() = default;

  std::vector<std::shared_ptr<SubMeshBase<TVertex>>> &get_sub_meshes()
  {
    return sub_meshes;
  }

  void add_sub_mesh(std::shared_ptr<SubMeshBase<TVertex>> sub_mesh)
  {
    sub_meshes.push_back(sub_mesh);
  }

private:
  std::vector<std::shared_ptr<SubMeshBase<TVertex>>> sub_meshes{};
};

} // namespace Fge

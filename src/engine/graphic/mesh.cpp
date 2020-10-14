#include "mesh.hpp"
#include "application.hpp"
#include "vertex_buffer_layout.hpp"

namespace Fge
{

SubMesh::SubMesh(const std::string &                       name,
                 std::shared_ptr<std::vector<VertexPNTBT>> vertices,
                 std::shared_ptr<std::vector<uint32_t>>    indices,
                 std::shared_ptr<MeshMaterial>             material)
    : name(name),
      vertices(vertices),
      indices(indices),
      material(material)
{
  auto app             = Application::get_instance();
  auto graphic_manager = app->get_graphic_manager();
  auto renderer        = graphic_manager->get_renderer();

  vertex_array = renderer->create_vertex_array();

  auto vertex_buffer = renderer->create_vertex_buffer_pntbt(*vertices);

  index_buffer = renderer->create_index_buffer(*indices);

  vertex_array->add_buffer(vertex_buffer);
  vertex_array->add_buffer(index_buffer);
}

SubMesh::SubMesh(const std::string &                       name,
                 std::shared_ptr<std::vector<VertexPNTBT>> vertices,
                 std::shared_ptr<std::vector<uint32_t>>    indices,
                 std::shared_ptr<MeshMaterial>             material,
                 std::shared_ptr<VertexArray>              vertex_array,
                 std::shared_ptr<IndexBuffer>              index_buffer)
    : name(name),
      vertices(vertices),
      indices(indices),
      vertex_array(vertex_array),
      index_buffer(index_buffer),
      material(material)
{
}

Mesh::Mesh(const std::vector<std::shared_ptr<SubMesh>> &sub_meshes)
    : sub_meshes(sub_meshes)
{
}

} // namespace Fge

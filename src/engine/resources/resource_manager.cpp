#include "resource_manager.hpp"
#include "application.hpp"
#include "graphic/mesh.hpp"
#include "graphic/mesh_material.hpp"
#include "mesh_importer.hpp"
#include <memory>

#define MESH_DIR "meshes"

namespace Fge
{

ResourceManager::ResourceManager()
{
  auto app                = Application::get_instance();
  auto filesystem_manager = app->get_file_manager();
  resource_path           = filesystem_manager->get_app_res_path();
}

std::shared_ptr<Mesh> create_mesh_instance(std::shared_ptr<Mesh> mesh)
{
  std::vector<std::shared_ptr<SubMesh>> new_sub_meshes;

  for (auto sub_mesh : mesh->get_sub_meshes())
  {
    auto name         = sub_mesh->get_name();
    auto vertices     = sub_mesh->get_vertices();
    auto indices      = sub_mesh->get_indices();
    auto vertex_array = sub_mesh->get_vertex_array();
    auto index_buffer = sub_mesh->get_index_buffer();
    auto material     = std::static_pointer_cast<MeshMaterial>(
        sub_mesh->get_material()->clone());

    auto new_sub_mesh = std::make_shared<SubMesh>(name,
                                                  vertices,
                                                  indices,
                                                  material,
                                                  vertex_array,
                                                  index_buffer);

    new_sub_meshes.push_back(new_sub_mesh);
  }

  return std::make_shared<Mesh>(new_sub_meshes);
}

std::shared_ptr<Mesh> ResourceManager::load_mesh(const std::string &filepath)
{
  std::shared_ptr<Mesh> mesh{};

  auto iter = mesh_cache.find(filepath);
  if (iter != mesh_cache.end())
  {
    mesh = iter->second;
  }
  else
  {
    mesh = import_mesh_from_file(resource_path / MESH_DIR / filepath);
    mesh_cache[filepath] = mesh;
  }

  auto mesh_instance = create_mesh_instance(mesh);

  return mesh_instance;
}

} // namespace Fge

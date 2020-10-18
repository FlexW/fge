#include <stb_image.h>

#include "application.hpp"
#include "graphic/mesh.hpp"
#include "graphic/mesh_material.hpp"
#include "graphic/skinned_mesh.hpp"
#include "graphic/texture.hpp"
#include "mesh_importer.hpp"
#include "resource_manager.hpp"
#include "skinned_mesh_importer.hpp"
#include "util/assert.hpp"

#define MESH_DIR    "meshes"
#define TEXTURE_DIR "textures"

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

std::shared_ptr<SkinnedMesh>
create_mesh_instance(std::shared_ptr<SkinnedMesh> mesh)
{
  std::vector<std::shared_ptr<SkinnedSubMesh>> new_sub_meshes;

  for (auto sub_mesh : mesh->get_sub_meshes())
  {
    auto name         = sub_mesh->get_name();
    auto vertices     = sub_mesh->get_vertices();
    auto indices      = sub_mesh->get_indices();
    auto vertex_array = sub_mesh->get_vertex_array();
    auto index_buffer = sub_mesh->get_index_buffer();
    auto material     = std::static_pointer_cast<MeshMaterial>(
        sub_mesh->get_material()->clone());

    auto new_sub_mesh = std::make_shared<SkinnedSubMesh>(name,
                                                         vertices,
                                                         indices,
                                                         material,
                                                         vertex_array,
                                                         index_buffer);

    new_sub_meshes.push_back(new_sub_mesh);
  }

  return std::make_shared<SkinnedMesh>(new_sub_meshes, mesh->get_skeleton());
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
    mesh =
        import_mesh_from_file((resource_path / MESH_DIR / filepath).string());
    mesh_cache[filepath] = mesh;
  }

  auto mesh_instance = create_mesh_instance(mesh);

  return mesh_instance;
}

std::shared_ptr<SkinnedMesh>
ResourceManager::load_skinned_mesh(const std::string &filepath)
{
  std::shared_ptr<SkinnedMesh> mesh{};

  auto iter = skinned_mesh_cache.find(filepath);
  if (iter != skinned_mesh_cache.end())
  {
    mesh = iter->second;
  }
  else
  {
    mesh = import_skinned_mesh_from_file(
        (resource_path / MESH_DIR / filepath).string());
    skinned_mesh_cache[filepath] = mesh;
  }

  auto mesh_instance = create_mesh_instance(mesh);

  return mesh_instance;
}

std::shared_ptr<Texture2D> load_texture2d_from_file(const std::string &filepath,
                                                    bool               flip)
{
  if (flip)
  {
    stbi_set_flip_vertically_on_load(true);
  }
  int32_t        width, height, nr_channels;
  unsigned char *data =
      stbi_load(filepath.c_str(), &width, &height, &nr_channels, 0);

  if (!data)
  {
    throw std::runtime_error("Could not load texture " + filepath);
  }

  ImageFormat format = ImageFormat::Rgb;

  switch (nr_channels)
  {
  case 1:
    format = ImageFormat::Red;
    break;
  case 3:
    format = ImageFormat::Rgb;
    break;
  case 4:
    format = ImageFormat::Rgba;
    break;
  default:
    FGE_FAIL("No such channel");
  }

  Texture2DConfig texture_config{};
  texture_config.name            = filepath;
  texture_config.width           = width;
  texture_config.height          = height;
  texture_config.data            = data;
  texture_config.format          = format;
  texture_config.wrap_s          = WrapMode::Repeat;
  texture_config.wrap_t          = WrapMode::Repeat;
  texture_config.filter_min      = FilterMode::LinearMipmapLinear;
  texture_config.filter_max      = FilterMode::Linear;
  texture_config.pixel_data_type = PixelDataType::UnsignedByte;
  texture_config.generate_mipmap = true;

  auto app = Application::get_instance();

  return app->get_graphic_manager()->get_renderer()->create_texture2d(
      texture_config);
}

std::shared_ptr<Texture2D>
ResourceManager::load_texture2d(const std::string &filepath, bool flip)
{
  auto iter = texture2d_cache.find(filepath);
  if (iter != texture2d_cache.end())
  {
    return iter->second;
  }

  auto texture = load_texture2d_from_file(
      (resource_path / TEXTURE_DIR / filepath).string(),
      flip);

  texture2d_cache[filepath] = texture;

  return texture;
}

} // namespace Fge

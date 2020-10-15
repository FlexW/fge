#pragma once

#include "graphic/mesh.hpp"
#include "graphic/texture.hpp"
#include "std.hpp"

namespace Fge
{

class ResourceManager
{
public:
  ResourceManager();

  std::shared_ptr<Mesh> load_mesh(const std::string &filepath);

  std::shared_ptr<Texture2D> load_texture2d(const std::string &filepath,
                                            bool               flip = false);

private:
  std::filesystem::path resource_path;

  std::unordered_map<std::string, std::shared_ptr<Mesh>> mesh_cache;

  std::unordered_map<std::string, std::shared_ptr<Texture2D>> texture2d_cache;
};

} // namespace Fge

#pragma once

#include "std.hpp"

namespace Fge
{

class Mesh;
class SkinnedMesh;
class Texture2D;

class ResourceManager
{
public:
  ResourceManager();

  std::shared_ptr<Mesh> load_mesh(const std::string &filepath);

  std::shared_ptr<SkinnedMesh> load_skinned_mesh(const std::string &filepath);

  std::shared_ptr<Texture2D> load_texture2d(const std::string &filepath,
                                            bool               flip = false);

private:
  std::filesystem::path resource_path;

  std::unordered_map<std::string, std::shared_ptr<Mesh>> mesh_cache;

  std::unordered_map<std::string, std::shared_ptr<SkinnedMesh>>
      skinned_mesh_cache;

  std::unordered_map<std::string, std::shared_ptr<Texture2D>> texture2d_cache;
};

} // namespace Fge

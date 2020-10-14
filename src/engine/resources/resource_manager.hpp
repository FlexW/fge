#pragma once

#include "graphic/mesh.hpp"
#include "std.hpp"
#include <string>
#include <unordered_map>

namespace Fge
{

class ResourceManager
{
public:
  ResourceManager();

  std::shared_ptr<Mesh> load_mesh(const std::string &filepath);

private:
  std::filesystem::path resource_path;

  std::unordered_map<std::string, std::shared_ptr<Mesh>> mesh_cache;
};

} // namespace Fge

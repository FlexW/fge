#pragma once

#include "std.hpp"

namespace Fge
{

class ResourceManager
{
public:
  ResourceManager();

private:
  std::filesystem::path resource_path;
};

} // namespace Fge

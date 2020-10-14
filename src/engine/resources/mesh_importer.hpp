#pragma once

#include "graphic/mesh.hpp"
#include "std.hpp"

namespace Fge
{

std::shared_ptr<Mesh> import_mesh_from_file(const std::string &filename);

} // namespace Fge

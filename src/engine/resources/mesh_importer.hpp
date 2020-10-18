#pragma once

#include "std.hpp"

namespace Fge
{

class Mesh;

std::shared_ptr<Mesh> import_mesh_from_file(const std::string &filename);

} // namespace Fge

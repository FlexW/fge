#pragma once

#include "std.hpp"

namespace Fge
{
class SkinnedMesh;

std::shared_ptr<SkinnedMesh>
import_skinned_mesh_from_file(const std::string &filename);

} // namespace Fge

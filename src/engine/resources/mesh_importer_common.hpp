#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "graphic/default_material.hpp"

namespace Fge
{
std::shared_ptr<DefaultMaterial> load_material(const aiScene *ai_scene,
                                               aiMesh *       ai_mesh);
}

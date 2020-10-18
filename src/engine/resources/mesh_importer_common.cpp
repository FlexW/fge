#include "mesh_importer_common.hpp"
#include "application.hpp"
#include "log/log.hpp"

namespace Fge
{

std::shared_ptr<Texture2D> load_texture(aiMaterial *  ai_material,
                                        aiTextureType ai_texture_type,
                                        bool          flip = false)
{
  uint32_t texture_count = ai_material->GetTextureCount(ai_texture_type);
  if (texture_count == 0)
  {
    return nullptr;
  }
  else if (texture_count > 1)
  {
    warning("MeshLoader",
            "Mesh has more than one texture defined. Can just handle one.");
  }

  aiString path;
  ai_material->GetTexture(ai_texture_type, 0, &path);

  auto app         = Application::get_instance();
  auto res_manager = app->get_resource_manager();

  return res_manager->load_texture2d(path.C_Str(), flip);
}

std::shared_ptr<DefaultMaterial> load_material(const aiScene *ai_scene,
                                               aiMesh *       ai_mesh)
{
  auto material = std::make_shared<DefaultMaterial>();

  aiMaterial *ai_material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];

  auto ambient_tex  = load_texture(ai_material, aiTextureType_AMBIENT, true);
  auto diffuse_tex  = load_texture(ai_material, aiTextureType_DIFFUSE, true);
  auto specular_tex = load_texture(ai_material, aiTextureType_SPECULAR, true);

  if (ambient_tex)
  {
    material->set_ambient_texture(ambient_tex);
  }
  if (diffuse_tex)
  {
    material->set_diffuse_texture(diffuse_tex);
  }
  if (specular_tex)
  {
    material->set_specular_texture(specular_tex);
  }

  material->set_specular_power(200.0f);

  return material;
}

} // namespace Fge

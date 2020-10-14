#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "graphic/default_material.hpp"
#include "graphic/mesh.hpp"
#include "graphic/vertices.hpp"
#include "mesh_importer.hpp"
#include "util/assert.hpp"

namespace Fge
{

void do_load_mesh(const aiScene *                        ai_scene,
                  aiNode *                               ai_node,
                  aiMatrix4x4 &                          parent_transform,
                  std::vector<std::shared_ptr<SubMesh>> &sub_meshes)
{
  auto transform = parent_transform * ai_node->mTransformation;

  for (unsigned i = 0; i < ai_node->mNumMeshes; ++i)
  {
    auto ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];
    std::cout << "Found mesh " << ai_mesh->mName.C_Str() << " in node "
              << ai_node->mName.C_Str() << std::endl;

    // Load vertices
    auto vertices = std::make_shared<std::vector<VertexPNTBT>>();
    vertices->reserve(ai_mesh->mNumVertices);

    for (unsigned j = 0; j < ai_mesh->mNumVertices; ++j)
    {
      VertexPNTBT vertex;

      const auto ai_vertex = transform * ai_mesh->mVertices[j];
      vertex.position      = glm::vec3(ai_vertex.x, ai_vertex.y, ai_vertex.z);

      if (ai_mesh->HasNormals())
      {
        const auto ai_normal = transform * ai_mesh->mNormals[j];
        vertex.normal        = glm::vec3(ai_normal.x, ai_normal.y, ai_normal.z);
      }

      if (ai_mesh->HasTangentsAndBitangents())
      {
        const auto ai_tangent   = transform * ai_mesh->mTangents[j];
        const auto ai_bitangent = transform * ai_mesh->mBitangents[j];

        vertex.tangent = glm::vec3(ai_tangent.x, ai_tangent.y, ai_tangent.z);
        vertex.bitangent =
            glm::vec3(ai_bitangent.x, ai_bitangent.y, ai_bitangent.z);
      }

      if (ai_mesh->HasTextureCoords(0))
      {
        vertex.tex_coord = glm::vec2(ai_mesh->mTextureCoords[0][j].x,
                                     ai_mesh->mTextureCoords[0][j].y);
      }

      if (ai_mesh->HasTextureCoords(1))
      {
        std::cout << "WARNING: Vertex has more than one texture coordinate"
                  << std::endl;
      }

      vertices->push_back(vertex);
    }

    // Load indices
    std::cout << "Load " << ai_mesh->mNumFaces << " faces in mesh "
              << ai_mesh->mName.C_Str() << std::endl;
    auto indices = std::make_shared<std::vector<unsigned>>();
    indices->reserve(ai_mesh->mNumFaces * 3);
    for (unsigned j = 0; j < ai_mesh->mNumFaces; ++j)
    {
      const auto ai_face = ai_mesh->mFaces[j];
      if (ai_face.mNumIndices != 3)
      {
        throw std::runtime_error(
            "Face has not three indices. Only three allowed");
      }
      indices->push_back(ai_face.mIndices[0]);
      indices->push_back(ai_face.mIndices[1]);
      indices->push_back(ai_face.mIndices[2]);
    }

    auto material = std::make_shared<DefaultMaterial>();

    auto sub_mesh = std::make_shared<SubMesh>(ai_mesh->mName.C_Str(),
                                              vertices,
                                              indices,
                                              material);
    sub_meshes.emplace_back(sub_mesh);
  }

  for (unsigned i = 0; i < ai_node->mNumChildren; ++i)
  {
    do_load_mesh(ai_scene, ai_node->mChildren[i], transform, sub_meshes);
  }
}

std::shared_ptr<Mesh> load_mesh(const aiScene *ai_scene)
{
  aiMatrix4x4 transform;
  FGE_ASSERT(transform.IsIdentity());

  std::vector<std::shared_ptr<SubMesh>> sub_meshes;
  do_load_mesh(ai_scene, ai_scene->mRootNode, transform, sub_meshes);

  return std::make_shared<Mesh>(sub_meshes);
}

std::shared_ptr<Mesh> import_mesh_from_file(const std::string &filename)
{
  Assimp::Importer importer;
  const aiScene *  ai_scene =
      importer.ReadFile(filename,
                        aiProcess_Triangulate |
                            /*aiProcess_FlipUVs |*/ aiProcess_GenNormals |
                            aiProcess_CalcTangentSpace);

  if (!ai_scene || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !ai_scene->mRootNode)
  {
    throw std::runtime_error(std::string("Assimp could not load model: ") +
                             importer.GetErrorString());
  }

  auto mesh = load_mesh(ai_scene);

  return mesh;
}

} // namespace Fge

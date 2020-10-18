#include "skinned_mesh_importer.hpp"
#include "application.hpp"
#include "graphic/default_material.hpp"
#include "graphic/skinned_mesh.hpp"
#include "mesh_importer_common.hpp"
#include "util/assert.hpp"

namespace Fge
{

static inline glm::mat4 mat4_cast(const aiMatrix4x4 &m)
{
  return glm::transpose(glm::make_mat4(&m.a1));
}

static inline glm::vec3 vec3_cast(const aiVector3D &v)
{
  return glm::vec3(v.x, v.y, v.z);
}

static inline glm::quat quat_cast(const aiQuaternion &q)
{
  return glm::quat(q.w, q.x, q.y, q.z);
}

void do_create_ai_node_necessary_map(
    aiNode *                            ai_node,
    std::unordered_map<aiNode *, bool> &ai_nodes_map)
{
  ai_nodes_map[ai_node] = false;
  std::cout << "Found ai node: " << ai_node->mName.C_Str() << std::endl;

  for (uint32_t i = 0; i < ai_node->mNumChildren; ++i)
  {
    do_create_ai_node_necessary_map(ai_node->mChildren[i], ai_nodes_map);
  }
}

std::unordered_map<aiNode *, bool>
create_ai_node_necessary_map(const aiScene *const ai_scene)
{
  std::unordered_map<aiNode *, bool> ai_nodes_map;

  do_create_ai_node_necessary_map(ai_scene->mRootNode, ai_nodes_map);

  return ai_nodes_map;
}

aiNode *do_find_ai_node_by_name(aiNode *ai_node, const std::string &name)
{
  if (std::string(ai_node->mName.C_Str()) == name)
  {
    return ai_node;
  }

  for (uint32_t i = 0; i < ai_node->mNumChildren; ++i)
  {
    auto ai_found_node = do_find_ai_node_by_name(ai_node->mChildren[i], name);

    if (ai_found_node)
    {
      return ai_found_node;
    }
  }

  return nullptr;
}

aiNode *find_ai_node_by_name(const aiScene *    ai_scene,
                             const std::string &node_name)
{
  return do_find_ai_node_by_name(ai_scene->mRootNode, node_name);
}

void do_mark_parent_ai_nodes_necessary(
    aiNode *                            ai_node,
    aiNode *                            ai_mesh_node,
    aiNode *                            ai_mesh_parent_node,
    std::unordered_map<aiNode *, bool> &ai_nodes_map)
{
  if (ai_node == ai_mesh_node || ai_node == ai_mesh_parent_node)
  {
    return;
  }

  ai_nodes_map[ai_node] = true;
  std::cout << "Found non bone node that is necessary for skeleton: "
            << ai_node->mName.C_Str() << std::endl;

  do_mark_parent_ai_nodes_necessary(ai_node->mParent,
                                    ai_mesh_node,
                                    ai_mesh_parent_node,
                                    ai_nodes_map);
}

void mark_parent_ai_nodes_necessary(
    aiNode *                            ai_bone_node,
    aiNode *                            ai_mesh_node,
    std::unordered_map<aiNode *, bool> &ai_nodes_map)
{
  auto ai_mesh_parent_node = ai_mesh_node->mParent;
  do_mark_parent_ai_nodes_necessary(ai_bone_node->mParent,
                                    ai_mesh_node,
                                    ai_mesh_parent_node,
                                    ai_nodes_map);
}

aiNode *do_find_ai_node_from_ai_mesh_by_name(const aiScene *    ai_scene,
                                             aiNode *           ai_node,
                                             const std::string &mesh_name)
{
  for (uint32_t i = 0; i < ai_node->mNumMeshes; ++i)
  {
    auto ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];
    if (std::string(ai_mesh->mName.C_Str()) == mesh_name)
    {
      return ai_node;
    }
  }

  for (uint32_t i = 0; i < ai_node->mNumChildren; ++i)
  {
    auto ai_node_found =
        do_find_ai_node_from_ai_mesh_by_name(ai_scene,
                                             ai_node->mChildren[i],
                                             mesh_name);

    if (ai_node_found)
    {
      return ai_node_found;
    }
  }

  return nullptr;
}

aiNode *find_ai_node_from_ai_mesh_by_name(const aiScene *    ai_scene,
                                          const std::string &mesh_name)
{
  return do_find_ai_node_from_ai_mesh_by_name(ai_scene,
                                              ai_scene->mRootNode,
                                              mesh_name);
}

void find_necessary_ai_nodes(const aiScene *                     ai_scene,
                             std::unordered_map<aiNode *, bool> &ai_nodes_map)
{
  auto ai_meshes = ai_scene->mMeshes;
  for (uint32_t i = 0; i < ai_scene->mNumMeshes; ++i)
  {
    auto ai_mesh = ai_meshes[i];

    for (uint32_t j = 0; j < ai_mesh->mNumBones; ++j)
    {
      auto        ai_bone      = ai_mesh->mBones[j];
      std::string bone_name    = ai_bone->mName.C_Str();
      auto        ai_bone_node = find_ai_node_by_name(ai_scene, bone_name);

      if (!ai_bone_node)
      {
        FGE_FAIL("Can not find node to bone");
      }

      std::cout << "Found necessary ai node: " << bone_name << std::endl;
      ai_nodes_map[ai_bone_node] = true;

      auto ai_mesh_node =
          find_ai_node_from_ai_mesh_by_name(ai_scene, ai_mesh->mName.C_Str());
      if (!ai_mesh_node)
      {
        std::cout << "Could not find ai node for ai mesh "
                  << ai_mesh->mName.C_Str() << std::endl;
      }
      else
      {
        mark_parent_ai_nodes_necessary(ai_bone_node,
                                       ai_mesh_node,
                                       ai_nodes_map);
      }
    }
  }
}

aiMatrix4x4 do_calculate_skeleton_transform(aiNode *ai_node)
{
  if (ai_node->mParent == nullptr)
  {
    return ai_node->mTransformation;
  }

  return do_calculate_skeleton_transform(ai_node->mParent) *
         ai_node->mTransformation;
}

aiMatrix4x4 calculate_skeleton_transform(const aiScene *    ai_scene,
                                         const std::string &name)
{
  aiNode *ai_node = find_ai_node_by_name(ai_scene, name);
  FGE_ASSERT(ai_node);

  if (ai_node->mParent == nullptr)
  {
    return ai_node->mTransformation;
  }

  return do_calculate_skeleton_transform(ai_node->mParent);
}

void do_build_bones(aiNode *                            ai_node,
                    std::unordered_map<aiNode *, bool> &ai_nodes_map,
                    int                                 parent_index,
                    std::vector<Skeleton::Bone> &       bones)
{
  auto iter = ai_nodes_map.find(ai_node);
  if (iter == ai_nodes_map.end())
  {
    FGE_FAIL("Ai node is not in ai nodes map");
  }

  auto bone_index = parent_index;

  auto necessary = iter->second;
  if (necessary)
  {
    Skeleton::Bone bone;
    bone.name            = ai_node->mName.C_Str();
    bone.parent_index    = parent_index;
    bone.local_bind_pose = mat4_cast(ai_node->mTransformation);

    bones.push_back(bone);
    std::cout << "Add node to skeleton: " << bone.name << std::endl;

    bone_index = bones.size() - 1;
  }
  else
  {
    std::cout << "Node " << ai_node->mName.C_Str()
              << " not necessary for skeleton" << std::endl;
  }

  for (uint32_t i = 0; i < ai_node->mNumChildren; ++i)
  {
    do_build_bones(ai_node->mChildren[i], ai_nodes_map, bone_index, bones);
  }
}

std::vector<Skeleton::Bone>
build_bones(const aiScene *                     ai_scene,
            std::unordered_map<aiNode *, bool> &ai_nodes_map)
{
  std::vector<Skeleton::Bone> bones;

  do_build_bones(ai_scene->mRootNode, ai_nodes_map, -1, bones);

  auto skeleton_transform =
      mat4_cast(calculate_skeleton_transform(ai_scene, bones[0].name));

  bones[0].local_bind_pose = skeleton_transform * bones[0].local_bind_pose;

  return bones;
}

std::vector<Animation> load_animations(const aiScene *ai_scene,
                                       Skeleton &     skeleton)
{
  std::vector<Animation> animations;

  // Traverse all animations and load animation tracks. A track is a sequence of
  // keyframes that animate a bone/node
  for (uint32_t i = 0; i < ai_scene->mNumAnimations; ++i)
  {
    std::vector<std::optional<Animation::BoneTransform>> tracks;
    tracks.resize(skeleton.get_bones_count());

    // Traverse all tracks
    const auto ai_animation = ai_scene->mAnimations[i];
    std::cout << "Found animation: " << ai_animation->mName.C_Str()
              << std::endl;
    for (uint32_t j = 0; j < ai_animation->mNumChannels; ++j)
    {
      const auto ai_node_anim = ai_animation->mChannels[j];
      const auto bone_index =
          skeleton.get_bone_index(ai_node_anim->mNodeName.C_Str());
      if (bone_index == -1)
      {
        std::cout << "Skeleton contains no bone named: " +
                         std::string(ai_node_anim->mNodeName.C_Str())
                  << std::endl;
        FGE_ASSERT(0);
      }

      std::cout << "Animation " << ai_animation->mName.C_Str() << " has track "
                << ai_node_anim->mNodeName.C_Str() << std::endl;

      Animation::BoneTransform bone_transform;

      // Load position keys
      for (uint32_t k = 0; k < ai_node_anim->mNumPositionKeys; ++k)
      {
        const auto ai_vector_key = ai_node_anim->mPositionKeys[k];

        Animation::BoneTranslation bone_translation;
        bone_translation.position = vec3_cast(ai_vector_key.mValue);
        bone_translation.time     = ai_vector_key.mTime;

        bone_transform.bone_translation.emplace_back(bone_translation);
      }

      // Load scale keys
      for (uint32_t k = 0; k < ai_node_anim->mNumScalingKeys; ++k)
      {
        const auto ai_vector_key = ai_node_anim->mScalingKeys[k];

        Animation::BoneScaling bone_scaling;
        bone_scaling.scale = vec3_cast(ai_vector_key.mValue);
        bone_scaling.time  = ai_vector_key.mTime;

        bone_transform.bone_scaling.emplace_back(bone_scaling);
      }

      // Load rotation keys
      for (uint32_t k = 0; k < ai_node_anim->mNumRotationKeys; ++k)
      {
        const auto ai_quat_key = ai_node_anim->mRotationKeys[k];

        Animation::BoneRotation bone_rotation;
        bone_rotation.rotation = quat_cast(ai_quat_key.mValue);
        bone_rotation.time     = ai_quat_key.mTime;

        bone_transform.bone_rotation.emplace_back(bone_rotation);
      }

      tracks[bone_index] = bone_transform;
    }

    const auto  duration         = ai_animation->mDuration;
    const auto  ticks_per_second = ai_animation->mTicksPerSecond;
    std::string animation_name   = ai_animation->mName.C_Str();
    if (animation_name == "")
    {
      animation_name = "Animation " + std::to_string(i);
      std::cout << "Animation has no name, name it: " << animation_name
                << std::endl;
    }

    Animation animation(animation_name, duration, ticks_per_second, tracks);

    animations.emplace_back(animation);
  }

  return animations;
}

void compute_global_inv_bind_poses(std::vector<Skeleton::Bone> &bones)
{
  if (bones.size() == 0)
  {
    return;
  }

  // First compute global bind pose

  // Root bone is special
  bones[0].global_inv_bind_pose = bones[0].local_bind_pose;
  for (uint32_t i = 1; i < bones.size(); ++i)
  {
    // Bones are ordered, parent bone comes always before child bone
    bones[i].global_inv_bind_pose =
        bones[bones[i].parent_index].global_inv_bind_pose *
        bones[i].local_bind_pose;
  }

  // Then invert everything
  for (auto &bone : bones)
  {
    bone.global_inv_bind_pose = glm::inverse(bone.global_inv_bind_pose);
  }
}

Skeleton load_skeleton(const aiScene *ai_scene)
{
  // Build up a map with all nodes that are in the scene
  // contained. This map saves for each node if this node is necessary
  // for the skeleton.
  auto ai_nodes_map = create_ai_node_necessary_map(ai_scene);

  // Find nodes that are necessary for the skeleton
  find_necessary_ai_nodes(ai_scene, ai_nodes_map);

  // Extract bones. First bone is the root bone.
  auto bones = build_bones(ai_scene, ai_nodes_map);

  std::cout << "Bones hierarchy:" << std::endl;
  for (auto &bone : bones)
  {
    std::cout << "name: " << bone.name
              << ", parent_index: " << bone.parent_index << std::endl;
  }

  compute_global_inv_bind_poses(bones);

  return Skeleton(bones);
}

void do_load_skinned_mesh(
    const aiScene *                               ai_scene,
    aiNode *                                      ai_node,
    aiMatrix4x4 &                                 parent_transform,
    Skeleton &                                    skeleton,
    std::vector<std::shared_ptr<SkinnedSubMesh>> &sub_meshes)
{
  auto transform = parent_transform * ai_node->mTransformation;

  for (uint32_t i = 0; i < ai_node->mNumMeshes; ++i)
  {
    auto ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];
    std::cout << "Found mesh " << ai_mesh->mName.C_Str() << " in node "
              << ai_node->mName.C_Str() << std::endl;

    // Load vertices
    auto vertices = std::make_shared<std::vector<VertexPNTBBWT>>();
    vertices->reserve(ai_mesh->mNumVertices);

    for (uint32_t j = 0; j < ai_mesh->mNumVertices; ++j)
    {
      VertexPNTBBWT vertex;

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

      vertices->emplace_back(vertex);
    }

    // Load skinning info

    // Stores how many bones already loaded for this vertex
    std::unordered_map<uint32_t, uint32_t> vertex_indices_to_bone_count_map;
    vertex_indices_to_bone_count_map.reserve(vertices->size());

    for (uint32_t j = 0; j < ai_mesh->mNumBones; ++j)
    {
      const auto ai_bone    = ai_mesh->mBones[j];
      const auto bone_index = skeleton.get_bone_index(ai_bone->mName.C_Str());
      FGE_ASSERT(bone_index != -1);

      for (uint32_t k = 0; k < ai_bone->mNumWeights; ++k)
      {
        const auto ai_vertex_weight = ai_bone->mWeights[k];
        auto       iter =
            vertex_indices_to_bone_count_map.find(ai_vertex_weight.mVertexId);

        // Are bones for this vertex already inserted?
        if (iter == vertex_indices_to_bone_count_map.end())
        {
          vertex_indices_to_bone_count_map[ai_vertex_weight.mVertexId] = 0;
          (*vertices)[ai_vertex_weight.mVertexId].skin_bones =
              glm::uvec4(bone_index, 0, 0, 0);
          (*vertices)[ai_vertex_weight.mVertexId].skin_weights =
              glm::vec4(ai_vertex_weight.mWeight, 0.0f, 0.0f, 0.0f);
        }
        else
        {
          const auto bone_count = iter->second + 1;
          // More than four bones?
          if (bone_count >= 4)
          {
            auto &vertex = (*vertices)[ai_vertex_weight.mVertexId];
            // Is weight greater?
            bool insert = false;
            for (uint32_t l = 0; l < 4; ++l)
            {
              if (vertex.skin_weights[l] < ai_vertex_weight.mWeight)
              {
                insert = true;
                break;
              }
            }

            // If is greater
            if (insert)
            {
              // Find smallest
              uint32_t smallest = 0;
              for (uint32_t l = 1; l < 4; ++l)
              {
                if (vertex.skin_weights[l] < vertex.skin_weights[smallest])
                {
                  smallest = l;
                }
              }

              // Insert it
              vertex.skin_weights[smallest] = ai_vertex_weight.mWeight;
              vertex.skin_bones[smallest]   = bone_index;
            }

            // Now rebalance
            float sum = 0.0f;
            for (uint32_t l = 0; l < 4; ++l)
            {
              sum += vertex.skin_weights[l];
            }
            for (uint32_t l = 0; l < 4; ++l)
            {
              vertex.skin_weights[l] /= sum;
            }
          }
          else
          {
            // Insert bone if not already four bones
            vertex_indices_to_bone_count_map[ai_vertex_weight.mVertexId] =
                bone_count;
            (*vertices)[ai_vertex_weight.mVertexId].skin_bones[bone_count] =
                bone_index;
            (*vertices)[ai_vertex_weight.mVertexId].skin_weights[bone_count] =
                ai_vertex_weight.mWeight;
          }
        }
      }
    }

    // Load indices
    std::cout << "Load " << ai_mesh->mNumFaces << " faces in mesh "
              << ai_mesh->mName.C_Str() << std::endl;
    auto indices = std::make_shared<std::vector<uint32_t>>();
    indices->reserve(ai_mesh->mNumFaces * 3);
    for (uint32_t j = 0; j < ai_mesh->mNumFaces; ++j)
    {
      const auto ai_face = ai_mesh->mFaces[j];
      if (ai_face.mNumIndices != 3)
      {
        throw std::runtime_error(
            "Face has not three indices. Only three allowed");
      }
      indices->emplace_back(ai_face.mIndices[0]);
      indices->emplace_back(ai_face.mIndices[1]);
      indices->emplace_back(ai_face.mIndices[2]);
    }

    auto material = load_material(ai_scene, ai_mesh);

    auto sub_mesh = std::make_shared<SkinnedSubMesh>(ai_mesh->mName.C_Str(),
                                                     vertices,
                                                     indices,
                                                     material);

    sub_meshes.emplace_back(sub_mesh);
  }

  for (uint32_t i = 0; i < ai_node->mNumChildren; ++i)
  {
    do_load_skinned_mesh(ai_scene,
                         ai_node->mChildren[i],
                         transform,
                         skeleton,
                         sub_meshes);
  }
}

std::shared_ptr<SkinnedMesh> load_skinned_mesh(const aiScene *ai_scene,
                                               Skeleton &     skeleton)
{
  aiMatrix4x4 transform;
  FGE_ASSERT(transform.IsIdentity());

  std::vector<std::shared_ptr<SkinnedSubMesh>> sub_meshes;
  do_load_skinned_mesh(ai_scene,
                       ai_scene->mRootNode,
                       transform,
                       skeleton,
                       sub_meshes);

  return std::make_shared<SkinnedMesh>(sub_meshes, skeleton);
}

std::shared_ptr<SkinnedMesh>
import_skinned_mesh_from_file(const std::string &filename)
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

  auto skeleton = load_skeleton(ai_scene);

  auto animations = load_animations(ai_scene, skeleton);

  for (auto &animation : animations)
  {
    skeleton.add_animation(animation);
  }

  auto mesh = load_skinned_mesh(ai_scene, skeleton);

  return mesh;
}

} // namespace Fge

#include "animation.hpp"
#include "util/assert.hpp"

namespace Fge
{

std::size_t Animation::BoneTransform::find_scaling(double time) const
{
  FGE_ASSERT(bone_scaling.size() > 0);

  for (unsigned i = 0; i < bone_scaling.size() - 1; ++i)
  {
    if (time < bone_scaling[i + 1].time)
    {
      return i;
    }
  }

  FGE_FAIL("No scaling found");
}

std::size_t Animation::BoneTransform::find_rotation(double time) const
{
  FGE_ASSERT(bone_rotation.size() > 0);

  for (unsigned i = 0; i < bone_rotation.size() - 1; ++i)
  {
    if (time < bone_rotation[i + 1].time)
    {
      return i;
    }
  }

  FGE_FAIL("No rotation found");
}

std::size_t Animation::BoneTransform::find_translation(double time) const
{
  FGE_ASSERT(bone_translation.size() > 0);

  for (unsigned i = 0; i < bone_translation.size() - 1; ++i)
  {
    if (time < bone_translation[i + 1].time)
    {
      return i;
    }
  }

  FGE_FAIL("No translation found");
}

glm::quat Animation::BoneTransform::interpolate_rotation(double time) const
{
  if (bone_rotation.size() == 1)
  {
    return bone_rotation[0].rotation;
  }

  const auto rotation_index      = find_rotation(time);
  const auto rotation_index_next = rotation_index + 1;
  FGE_ASSERT(rotation_index_next < bone_rotation.size());

  const auto delta_time = bone_rotation[rotation_index_next].time -
                          bone_rotation[rotation_index].time;
  float factor = (time - bone_rotation[rotation_index].time) / delta_time;
  // Maybe the animation data does not start at time zero
  if (factor <= 0.0f)
  {
    factor = 0.0f;
  }
  FGE_ASSERT(factor >= 0.0f && factor <= 1.0f);

  const auto start = bone_rotation[rotation_index].rotation;
  const auto end   = bone_rotation[rotation_index_next].rotation;

  return glm::normalize(glm::slerp(start, end, factor));
}

glm::vec3 Animation::BoneTransform::interpolate_position(double time) const
{
  if (bone_translation.size() == 1)
  {
    return bone_translation[0].position;
  }

  const auto translation_index      = find_translation(time);
  const auto translation_index_next = translation_index + 1;
  FGE_ASSERT(translation_index_next < bone_translation.size());

  const auto delta_time = bone_translation[translation_index_next].time -
                          bone_translation[translation_index].time;
  auto factor = (time - bone_translation[translation_index].time) / delta_time;

  // Maybe the animation data does not start at time zero
  if (factor <= 0.0f)
  {
    factor = 0.0f;
  }
  FGE_ASSERT(factor >= 0.0f && factor <= 1.0f);

  const auto start = bone_translation[translation_index].position;
  const auto end   = bone_translation[translation_index_next].position;
  const auto delta = end - start;
  return start +
         glm::vec3(factor * delta.x, factor * delta.y, factor * delta.z);
}

glm::vec3 Animation::BoneTransform::interpolate_scale(double time) const
{
  if (bone_scaling.size() == 1)
  {
    return bone_scaling[0].scale;
  }

  const auto scaling_index      = find_scaling(time);
  const auto scaling_index_next = scaling_index + 1;
  FGE_ASSERT(scaling_index_next < bone_scaling.size());

  const auto delta_time =
      bone_scaling[scaling_index_next].time - bone_scaling[scaling_index].time;
  auto factor = (time - bone_scaling[scaling_index].time) / delta_time;
  // Maybe the animation data does not start at time zero
  if (factor <= 0.0f)
  {
    factor = 0.0f;
  }
  FGE_ASSERT(factor >= 0.0f && factor <= 1.0f);

  const auto start = bone_scaling[scaling_index].scale;
  const auto end   = bone_scaling[scaling_index_next].scale;
  const auto delta = end - start;
  return start +
         glm::vec3(factor * delta.x, factor * delta.y, factor * delta.z);
}

glm::mat4 Animation::BoneTransform::interpolate(double time) const
{
  const auto position = interpolate_position(time);
  const auto rotation = interpolate_rotation(time);
  const auto scale    = interpolate_scale(time);

  const auto rotation_mat  = glm::toMat4(rotation);
  const auto scale_mat     = glm::scale(glm::mat4(1.0f), scale);
  const auto translate_mat = glm::translate(glm::mat4(1.0f), position);

  return translate_mat * rotation_mat * scale_mat;
}

Animation::Animation(const std::string &name,
                     double             duration,
                     double             ticks_per_second,
                     const std::vector<std::optional<BoneTransform>> &tracks)
    : name(name),
      duration(duration),
      ticks_per_second(ticks_per_second),
      tracks(tracks)
{
}

} // namespace Fge

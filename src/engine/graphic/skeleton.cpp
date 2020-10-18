#include "skeleton.hpp"
#include "util/assert.hpp"

namespace Fge
{

Skeleton::Skeleton(const std::vector<Bone> &bones) : bones(bones)
{
  transforms.resize(bones.size());

  transforms_identity.reserve(bones.size());
  for (unsigned i = 0; i < bones.size(); ++i)
  {
    transforms_identity.push_back(glm::mat4(1.0f));
  }
}

int Skeleton::get_bone_index(const std::string &name)
{
  for (unsigned i = 0; i < bones.size(); ++i)
  {
    if (name == bones[i].name)
    {
      return i;
    }
  }

  return -1;
}

void Skeleton::play_animation(const std::string &name,
                              double             current_time_in_seconds)
{
  int animation_index = get_index_of_animation_by_name(name);
  if (animation_index == -1)
  {
    return;
  }

  animation_start_time   = current_time_in_seconds;
  endless_animation      = false;
  active_animation_index = animation_index;
}

void Skeleton::play_animation_endless(const std::string &name,
                                      double current_time_in_seconds)
{
  int animation_index = get_index_of_animation_by_name(name);
  if (animation_index == -1)
  {
    return;
  }

  animation_start_time   = current_time_in_seconds;
  endless_animation      = true;
  active_animation_index = animation_index;
}

int Skeleton::get_index_of_animation_by_name(const std::string &name)
{
  for (unsigned i = 0; i < animations.size(); ++i)
  {
    if (animations[i].get_name() == name)
    {
      return i;
    }
  }
  return -1;
}

void Skeleton::stop_current_animation()
{
  active_animation_index = -1;
  animation_start_time   = 0.0f;
  endless_animation      = false;
}

const std::vector<glm::mat4> &
Skeleton::compute_bone_transforms(double current_time_in_seconds)
{
  if (active_animation_index == -1)
  {
    return transforms_identity;
  }

  FGE_ASSERT(0 <= active_animation_index &&
             static_cast<std::size_t>(active_animation_index) <
                 animations.size());

  const auto &animation = animations[active_animation_index];

  double ticks_per_second = animation.get_ticks_per_second();
  ticks_per_second        = ticks_per_second != 0 ? ticks_per_second : 25.0f;
  ticks_per_second *= animation_speed;

  const auto current_time_in_ticks =
      (current_time_in_seconds - animation_start_time) * ticks_per_second;

  // Is animation over?
  if (!endless_animation &&
      current_time_in_ticks >= animation.get_duration())
  {
    active_animation_index = -1;
    animation_start_time   = 0.0f;
    endless_animation      = false;
    return transforms_identity;
  }

  const auto animation_time =
      std::fmod(current_time_in_ticks, animation.get_duration());

  // Compute the bone transformations
  // Root node is special
  const auto &track = animation.get_track(0);
  if (track.has_value())
  {
    transforms[0] = track.value().interpolate(animation_time);
  }
  else
  {
    transforms[0] = bones[0].local_bind_pose;
  }

  for (unsigned i = 1; i < bones.size(); ++i)
  {
    auto &      bone  = bones[i];
    const auto &track = animation.get_track(i);

    if (track.has_value())
    {
      transforms[i] = transforms[bone.parent_index] *
                      track.value().interpolate(animation_time);
    }
    else
    {
      transforms[i] = transforms[bone.parent_index] * bone.local_bind_pose;
    }
  }

  for (unsigned i = 0; i < bones.size(); ++i)
  {
    const auto &bone = bones[i];
    transforms[i]    = transforms[i] * bone.global_inv_bind_pose;
  }

  return transforms;
}

void Skeleton::add_animation(Animation &animation)
{
  animations.push_back(animation);
}

} // namespace Fge

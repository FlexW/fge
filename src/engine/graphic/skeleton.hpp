#pragma once

#include "animation.hpp"
#include "math/math.hpp"
#include "std.hpp"

namespace Fge
{

class Skeleton
{
public:
  struct Bone
  {
    std::string name;
    int         parent_index = -1;
    glm::mat4   local_bind_pose;
    glm::mat4   global_inv_bind_pose;
  };

  Skeleton(const std::vector<Bone> &bones);

  int get_bone_index(const std::string &name);

  unsigned get_bones_count() const { return bones.size(); }

  void play_animation(const std::string &name, double current_time_in_seconds);

  void play_animation_endless(const std::string &name,
                              double             current_time_in_seconds);

  void stop_current_animation();

  const std::vector<glm::mat4> &
  compute_bone_transforms(double current_time_in_seconds);

  void add_animation(Animation &animation);

private:
  std::vector<Bone> bones;

  std::vector<glm::mat4> transforms;
  std::vector<glm::mat4> transforms_identity;

  double                          animation_speed        = 1.0f;
  double                          animation_start_time   = 0.0f;
  bool                            endless_animation      = false;
  int                             active_animation_index = -1;
  std::vector<Animation>          animations;

  int get_index_of_animation_by_name(const std::string &name);
};

} // namespace Fge

#pragma once

#include "math/math.hpp"
#include "std.hpp"

namespace Fge
{

class Animation
{
public:
  struct BoneRotation
  {
    double    time;
    glm::quat rotation;
  };

  struct BoneTranslation
  {
    double    time;
    glm::vec3 position;
  };

  struct BoneScaling
  {
    double    time;
    glm::vec3 scale;
  };

  struct BoneTransform
  {
    std::vector<BoneRotation>    bone_rotation;
    std::vector<BoneTranslation> bone_translation;
    std::vector<BoneScaling>     bone_scaling;

    glm::mat4 interpolate(double time) const;

  private:
    std::size_t find_scaling(double time) const;

    std::size_t find_rotation(double time) const;

    std::size_t find_translation(double time) const;

    glm::quat interpolate_rotation(double time) const;

    glm::vec3 interpolate_position(double time) const;

    glm::vec3 interpolate_scale(double time) const;
  };

  Animation(const std::string &                              name,
            double                                           duration,
            double                                           ticks_per_second,
            const std::vector<std::optional<BoneTransform>> &tracks);

  std::string get_name() const { return name; }

  double get_duration() const { return duration; }

  double get_ticks_per_second() const { return ticks_per_second; }

  const std::vector<std::optional<BoneTransform>> &get_tracks() const
  {
    return tracks;
  }

  const std::optional<BoneTransform> &get_track(unsigned index) const
  {
    return tracks[index];
  }

private:
  std::string name;

  double duration;

  double ticks_per_second;

  std::vector<std::optional<BoneTransform>> tracks;
};

} // namespace Fge

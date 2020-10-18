#pragma once

#include "math/math.hpp"
#include "scene/component.hpp"

namespace Fge
{

class FollowCameraComponent : public Component
{
public:
  FollowCameraComponent(
      Actor *            owner,
      int                update_order = 1000,
      const std::string &type_name    = "Fge::FollowCameraComponent");

  void set_horz_dist(float dist) { horz_dist = dist; }

  void set_vert_dist(float dist) { vert_dist = dist; }

  void set_target_dist(float dist) { target_dist = dist; }

  void set_spring_constant(float spring) { spring_constant = spring; }

protected:
  void update(float delta_time) override;

private:
  float horz_dist   = 10.0f;
  float vert_dist   = 10.0f;
  float target_dist = 10.0f;
  float spring_constant = 1.0f;

  glm::vec3 velocity{};

  glm::vec3 camera_position{};
};

} // namespace Fge

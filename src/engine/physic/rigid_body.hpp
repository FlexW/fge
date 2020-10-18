#pragma once

#include "physic/collision_shape.hpp"
#include "std.hpp"

namespace Fge
{

class RigidBody
{
public:
  RigidBody() = default;

  virtual ~RigidBody() = default;

  virtual void set_mass(float mass) = 0;

  virtual void
  set_collision_shape(std::shared_ptr<CollisionShape> collision_shape) = 0;

  virtual glm::vec3 get_position() = 0;

  virtual glm::quat get_rotation() = 0;

private:
  RigidBody(const RigidBody &other) = delete;

  void operator=(const RigidBody &other) = delete;
};

} // namespace Fge

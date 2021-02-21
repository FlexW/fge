#pragma once

#include "collision_shape.hpp"
#include "rigid_body.hpp"
#include "std.hpp"

namespace Fge
{

class PhysicWorld
{
public:
  virtual ~PhysicWorld() = default;

  virtual void update(float delta_time) = 0;

  virtual void render() = 0;

  virtual void terminate() = 0;

  virtual std::shared_ptr<SphereCollisionShape>
  create_sphere_collision_shape(float radius) = 0;

  virtual std::shared_ptr<BoxCollisionShape>
  create_box_collision_shape(float x, float y, float z) = 0;

  virtual std::shared_ptr<RigidBody>
  create_rigid_body(float                           mass,
                    std::shared_ptr<CollisionShape> collision_shape,
                    const glm::mat4 &               transform) = 0;
};

} // namespace Fge

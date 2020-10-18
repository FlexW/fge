#pragma once

#include "bullet.hpp"
#include "math/math.hpp"
#include "physic/collision_shape.hpp"
#include "physic/rigid_body.hpp"

namespace Fge::Bullet
{

class BulletRigidBody : public RigidBody
{
public:
  BulletRigidBody(btDiscreteDynamicsWorld *       dynamics_world,
                  float                           mass,
                  const glm::mat4 &               transform,
                  std::shared_ptr<CollisionShape> collision_shape);

  ~BulletRigidBody();

  void set_mass(float mass) override;

  void
  set_collision_shape(std::shared_ptr<CollisionShape> collision_shape) override;

  glm::vec3 get_position() override;

  glm::quat get_rotation() override;

private:
  std::shared_ptr<CollisionShape> collision_shape{};

  btDiscreteDynamicsWorld *dynamics_world{};
  btRigidBody *            body{};
};

} // namespace Fge::Bullet

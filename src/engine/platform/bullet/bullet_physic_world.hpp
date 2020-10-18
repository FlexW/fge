#pragma once

#include "bullet.hpp"
#include "physic/physic_world.hpp"

namespace Fge::Bullet
{

class BulletPhysicWorld : public PhysicWorld
{
public:
  BulletPhysicWorld();

  ~BulletPhysicWorld();

  void update(float delta_time) override;

  void render() override;

  void terminate() override;

  std::shared_ptr<SphereCollisionShape>
  create_sphere_collision_shape(float radius) override;

  std::shared_ptr<BoxCollisionShape>
  create_box_collision_shape(float x, float y, float z) override;

  std::shared_ptr<RigidBody>
  create_rigid_body(float                           mass,
                    std::shared_ptr<CollisionShape> collision_shape,
                    const glm::mat4 &               transform) override;

private:
  btDefaultCollisionConfiguration *    collision_configuration{};
  btCollisionDispatcher *              dispatcher{};
  btBroadphaseInterface *              overlapping_pair_cache{};
  btSequentialImpulseConstraintSolver *solver{};
  btDiscreteDynamicsWorld *            dynamics_world{};
};

} // namespace Fge::Bullet

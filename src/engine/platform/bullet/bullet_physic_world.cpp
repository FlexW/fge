#include "bullet_physic_world.hpp"
#include "bullet_collision_shape.hpp"
#include "bullet_rigid_body.hpp"
#include "math/math.hpp"
#include "physic/collision_shape.hpp"

#define GRAVITY -9.8

namespace Fge::Bullet
{

BulletPhysicWorld::BulletPhysicWorld()
{
  collision_configuration = new btDefaultCollisionConfiguration();
  dispatcher              = new btCollisionDispatcher(collision_configuration);
  overlapping_pair_cache  = new btDbvtBroadphase();
  solver                  = new btSequentialImpulseConstraintSolver();
  dynamics_world          = new btDiscreteDynamicsWorld(dispatcher,
                                               overlapping_pair_cache,
                                               solver,
                                               collision_configuration);

  dynamics_world->setGravity(btVector3(0, GRAVITY, 0));
}

BulletPhysicWorld::~BulletPhysicWorld()
{
  delete dynamics_world;
  delete solver;
  delete overlapping_pair_cache;
  delete dispatcher;
  delete collision_configuration;
}

void BulletPhysicWorld::update(float delta_time)
{
  dynamics_world->stepSimulation(delta_time);
}

std::shared_ptr<SphereCollisionShape>
BulletPhysicWorld::create_sphere_collision_shape(float radius)
{
  return std::make_shared<BulletSphereCollisionShape>(radius);
}

std::shared_ptr<BoxCollisionShape>
BulletPhysicWorld::create_box_collision_shape(float x, float y, float z)
{
  return std::make_shared<BulletBoxCollisionShape>(x, y, z);
}

std::shared_ptr<RigidBody> BulletPhysicWorld::create_rigid_body(
    float                           mass,
    std::shared_ptr<CollisionShape> collision_shape,
    const glm::mat4 &               transform)
{
  auto rigid_body = std::make_shared<BulletRigidBody>(dynamics_world,
                                                      mass,
                                                      transform,
                                                      collision_shape);

  return rigid_body;
}

void BulletPhysicWorld::render() {}

void BulletPhysicWorld::terminate() {}

} // namespace Fge::Bullet

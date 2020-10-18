#include "bullet_rigid_body.hpp"
#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "application.hpp"
#include "bullet_collision_shape.hpp"

namespace Fge::Bullet
{

BulletRigidBody::BulletRigidBody(
    btDiscreteDynamicsWorld *       dynamics_world,
    float                           mass,
    const glm::mat4 &               transform,
    std::shared_ptr<CollisionShape> collision_shape)
    : collision_shape(collision_shape),
      dynamics_world(dynamics_world)
{
  btTransform bt_transform;
  bt_transform.setFromOpenGLMatrix(glm::value_ptr(transform));

  btDefaultMotionState *motion_state = new btDefaultMotionState(bt_transform);

  auto bullet_collision_shape =
      std::dynamic_pointer_cast<BulletCollisionShape>(collision_shape);

  bool is_dynamic = (mass != 0.f);

  glm::vec3 local_inertia(0, 0, 0);
  if (is_dynamic)
  {
    local_inertia = collision_shape->calculate_local_inertia(mass);
  }
  btVector3 bt_local_inertia(local_inertia.x, local_inertia.y, local_inertia.z);

  btRigidBody::btRigidBodyConstructionInfo rb_info(
      mass,
      motion_state,
      bullet_collision_shape->get_bt_collision_shape(),
      bt_local_inertia);

  body = new btRigidBody(rb_info);

  dynamics_world->addRigidBody(body);

  body->setActivationState(ACTIVE_TAG);
}

BulletRigidBody::~BulletRigidBody()
{
  dynamics_world->removeRigidBody(body);
  delete body;
}

void BulletRigidBody::set_mass(float mass)
{
  dynamics_world->removeRigidBody(body);

  bool is_dynamic = (mass != 0.f);

  glm::vec3 local_inertia(0, 0, 0);
  if (is_dynamic)
  {
    local_inertia = collision_shape->calculate_local_inertia(mass);
  }
  btVector3 bt_local_inertia(local_inertia.x, local_inertia.y, local_inertia.z);

  body->setMassProps(btScalar(mass), bt_local_inertia);

  dynamics_world->addRigidBody(body);
  body->setActivationState(ACTIVE_TAG);
}

void BulletRigidBody::set_collision_shape(
    std::shared_ptr<CollisionShape> collision_shape)
{
  auto bullet_collision_shape =
      std::dynamic_pointer_cast<BulletCollisionShape>(collision_shape);

  body->setCollisionShape(bullet_collision_shape->get_bt_collision_shape());

  this->collision_shape = collision_shape;

  body->setActivationState(ACTIVE_TAG);
}

glm::vec3 BulletRigidBody::get_position()
{
  btTransform transform;
  if (body->getMotionState())
  {
    body->getMotionState()->getWorldTransform(transform);
  }
  else
  {
    transform = body->getWorldTransform();
  }

  return glm::vec3(transform.getOrigin().getX(),
                   transform.getOrigin().getY(),
                   transform.getOrigin().getZ());
}

glm::quat BulletRigidBody::get_rotation()
{
  btQuaternion rotation = body->getWorldTransform().getRotation();

  return glm::quat(rotation.getW(),
                   rotation.getX(),
                   rotation.getY(),
                   rotation.getZ());
}

} // namespace Fge::Bullet

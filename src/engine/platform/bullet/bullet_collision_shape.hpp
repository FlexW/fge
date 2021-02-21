#pragma once

#include "bullet.hpp"
#include "physic/collision_shape.hpp"

namespace Fge::Bullet
{

class BulletCollisionShape
{
public:
  virtual ~BulletCollisionShape() = default;

  virtual btCollisionShape *get_bt_collision_shape() = 0;
};

class BulletSphereCollisionShape : public SphereCollisionShape,
                                   public BulletCollisionShape
{
public:
  BulletSphereCollisionShape(float radius);

  ~BulletSphereCollisionShape();

  btCollisionShape *get_bt_collision_shape() override
  {
    return bt_sphere_shape;
  }

  glm::vec3 calculate_local_inertia(float mass) override
  {
    btVector3 inertia(0.0, 0.0, 0.0);
    bt_sphere_shape->calculateLocalInertia(mass, inertia);

    return glm::vec3(inertia.x(), inertia.y(), inertia.z());
  }

private:
  // float radius{};

  btSphereShape *bt_sphere_shape{};
};

class BulletBoxCollisionShape : public BoxCollisionShape,
                                public BulletCollisionShape
{
public:
  BulletBoxCollisionShape(float x, float y, float z);

  ~BulletBoxCollisionShape();

  btCollisionShape *get_bt_collision_shape() override { return bt_box_shape; }

  glm::vec3 calculate_local_inertia(float mass) override
  {
    btVector3 inertia(0.0, 0.0, 0.0);
    bt_box_shape->calculateLocalInertia(mass, inertia);

    return glm::vec3(inertia.x(), inertia.y(), inertia.z());
  }

private:
  // float x;
  // float y;
  // float z;

  btBoxShape *bt_box_shape{};
};

} // namespace Fge::Bullet

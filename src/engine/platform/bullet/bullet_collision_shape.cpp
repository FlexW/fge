#include "bullet_collision_shape.hpp"

namespace Fge::Bullet
{

BulletSphereCollisionShape::BulletSphereCollisionShape(float radius)
    : // radius(radius),
      bt_sphere_shape(new btSphereShape(btScalar(radius)))
{
}

BulletSphereCollisionShape::~BulletSphereCollisionShape()
{
  delete bt_sphere_shape;
}

BulletBoxCollisionShape::BulletBoxCollisionShape(float x, float y, float z)
    : // x(x),
      // y(y),
      // z(z),
      bt_box_shape(new btBoxShape(btVector3(x, y, z)))
{
}

BulletBoxCollisionShape::~BulletBoxCollisionShape() { delete bt_box_shape; }

} // namespace Fge::Bullet

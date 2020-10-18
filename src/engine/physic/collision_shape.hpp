#pragma once

#include "math/math.hpp"

namespace Fge
{

class CollisionShape
{
public:
  CollisionShape() = default;

  virtual ~CollisionShape() = default;

  virtual glm::vec3 calculate_local_inertia(float mass) = 0;

private:
  CollisionShape(const CollisionShape &other) = delete;

  void operator=(const CollisionShape &other) = delete;
};

class SphereCollisionShape : public CollisionShape
{
public:
};

class BoxCollisionShape : public CollisionShape
{
public:
};

} // namespace Fge

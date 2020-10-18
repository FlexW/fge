#pragma once

#include "physic/collision_shape.hpp"
#include "physic/rigid_body.hpp"
#include "scene/component.hpp"

namespace Fge
{

class RigidBodyComponent : public Component
{
public:
  RigidBodyComponent(Actor *            owner,
                     int                update_order = 1000,
                     const std::string &type_name = "Fge::RigidBodyComponent");

  void set_mass(float mass);

protected:
  float mass = 0.0f;

  std::shared_ptr<CollisionShape> collision_shape{};

  void create() override;

  void update(float delta_time) override;

  void render() override;

  void set_collision_shape(std::shared_ptr<CollisionShape> collision_shape);

  void create_rigid_body();

private:
  bool created = false;

  std::shared_ptr<RigidBody> rigid_body{};
};

} // namespace Fge

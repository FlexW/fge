#include "rigid_body_component.hpp"
#include "application.hpp"
#include "scene/component.hpp"

namespace Fge
{

RigidBodyComponent::RigidBodyComponent(Actor *            owner,
                                       int                update_order,
                                       const std::string &type_name)
    : Component(owner, update_order, type_name)
{
}

void RigidBodyComponent::create()
{
  created = true;
  create_rigid_body();
}

void RigidBodyComponent::update(float /*delta_time*/)
{
  if (!rigid_body)
  {
    return;
  }

  const auto position = rigid_body->get_position();
  const auto rotation = rigid_body->get_rotation();

  owner->set_position(position);
  owner->set_rotation(rotation);
}

void RigidBodyComponent::render() {}

void RigidBodyComponent::set_collision_shape(
    std::shared_ptr<CollisionShape> collision_shape)
{
  this->collision_shape = collision_shape;

  if (!rigid_body)
  {
    create_rigid_body();
  }

  if (rigid_body)
  {
    rigid_body->set_collision_shape(collision_shape);
  }
}

void RigidBodyComponent::set_mass(float mass)
{
  this->mass = mass;

  if (!rigid_body)
  {
    create_rigid_body();
    return;
  }

  if (rigid_body)
  {
    rigid_body->set_mass(mass);
  }
}

void RigidBodyComponent::create_rigid_body()
{
  if (!collision_shape || !created || rigid_body)
  {
    return;
  }

  auto physic_manager = Application::get_instance()->get_physic_manager();
  auto physic_world   = physic_manager->get_physic_world();

  auto world_transform = glm::translate(glm::mat4(1.0f), owner->get_position());
  world_transform *= glm::toMat4(owner->get_rotation());

  rigid_body =
      physic_world->create_rigid_body(mass, collision_shape, world_transform);
}

} // namespace Fge

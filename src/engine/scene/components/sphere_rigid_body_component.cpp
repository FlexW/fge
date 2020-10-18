#include "sphere_rigid_body_component.hpp"
#include "application.hpp"
#include "physic/collision_shape.hpp"

namespace Fge
{

SphereRigidBodyComponent::SphereRigidBodyComponent(Actor *owner,
                                                   int    update_order,
                                                   const std::string &type_name)
    : RigidBodyComponent(owner, update_order, type_name)
{
}

void SphereRigidBodyComponent::set_radius(float radius)
{
  auto physic_world =
      Application::get_instance()->get_physic_manager()->get_physic_world();

  auto cs = physic_world->create_sphere_collision_shape(radius);

  set_collision_shape(cs);
}

} // namespace Fge

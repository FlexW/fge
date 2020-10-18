#include "box_rigid_body_component.hpp"
#include "application.hpp"

namespace Fge
{

BoxRigidBodyComponent::BoxRigidBodyComponent(Actor *            owner,
                                             int                update_order,
                                             const std::string &type_name)
    : RigidBodyComponent(owner, update_order, type_name)
{
}

void BoxRigidBodyComponent::set_half_extents(float x, float y, float z)
{
  auto physic_world =
      Application::get_instance()->get_physic_manager()->get_physic_world();

  auto cs = physic_world->create_box_collision_shape(x, y, z);

  set_collision_shape(cs);
}

} // namespace Fge

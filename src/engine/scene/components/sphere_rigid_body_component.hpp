#pragma once

#include "rigid_body_component.hpp"

namespace Fge
{

class SphereRigidBodyComponent : public RigidBodyComponent
{
public:
  SphereRigidBodyComponent(
      Actor *            owner,
      int                update_order = 1000,
      const std::string &type_name    = "Fge::SphereRigidBodyComponent");

  void set_radius(float radius);
};

} // namespace Fge

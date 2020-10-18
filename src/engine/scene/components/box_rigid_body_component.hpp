#pragma once

#include "rigid_body_component.hpp"

namespace Fge
{

class BoxRigidBodyComponent : public RigidBodyComponent
{
public:
  BoxRigidBodyComponent(
      Actor *            owner,
      int                update_order = 1000,
      const std::string &type_name    = "Fge::BoxRigidBodyComponent");

  void set_half_extents(float x, float y, float z);
};

} // namespace Fge

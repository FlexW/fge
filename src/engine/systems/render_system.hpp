#pragma once

#include "ecs/fwd.hpp"
#include "scn/system.hpp"

namespace fge
{

class render_system : public scn::system
{
public:
  void update(ecs::registry &scene_registry, float delta_time) override;
};
}

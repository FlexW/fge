#pragma once

#include "ecs/fwd.hpp"
namespace fge::scn
{

class system
{
public:
  virtual void create(ecs::registry &scene_registry) = 0;

  virtual void update(ecs::registry &scene_registry, float delta_time) = 0;

  virtual void destroy(ecs::registry &scene_registry) = 0;
};

} // namespace fge::scn

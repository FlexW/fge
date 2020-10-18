#pragma once

#include "physic_world.hpp"

namespace Fge
{

class PhysicManager
{
public:
  void init();

  void update(float delta_time);

  void render();

  void terminate();

  std::shared_ptr<PhysicWorld> get_physic_world() { return physic_world; }

private:
  std::shared_ptr<PhysicWorld> physic_world{};
};

} // namespace Fge

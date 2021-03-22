#pragma once

#include "broadcast.hpp"
#include "ecs/fwd.hpp"
#include "ecs/registry.hpp"
#include "system.hpp"

namespace fge::scn
{

class scene
{
public:
  scene(broadcast *scene_broadcast);

  void create();

  void update();

  void destruct();

  void set_registry(ecs::registry &&registry);

  template <typename TSystem> void add_system()
  {
    systems.emplace_back(new TSystem);
  }

private:
  ecs::registry scene_registry;

  std::vector<std::unique_ptr<system>> systems;

  broadcast *scene_broadcast;
};
}

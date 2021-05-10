#include "scene.hpp"
#include "util/assert.hpp"

namespace fge::scn
{

scene::scene(broadcast *scene_broadcast) : scene_broadcast(scene_broadcast)
{
  FGE_ASSERT(scene_broadcast);
}

void scene::create()
{
  for (auto &system : systems)
  {
    system->create(scene_registry);
  }
}

void scene::update(float delta_time)
{
  for (auto &system : systems)
  {
    system->update(scene_registry, delta_time);
  }
}

void scene::destruct()
{
  for (auto &system : systems)
  {
    system->destroy(scene_registry);
  }
}

void scene::set_registry(ecs::registry &&registry)
{
  scene_registry = std::move(registry);
}

  } // namespace fge::scn

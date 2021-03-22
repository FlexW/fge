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
    system->create();
  }
}

void scene::update()
{
  for (auto &system : systems)
  {
    system->update();
  }
}

void scene::destruct()
{
  for (auto &system : systems)
  {
    system->destroy();
  }
}

void scene::set_registry(ecs::registry &&registry)
{
  scene_registry = std::move(registry);
}

  } // namespace fge::scn

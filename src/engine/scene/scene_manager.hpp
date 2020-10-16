#pragma once

#include "event/event.hpp"
#include "scene.hpp"
#include "std.hpp"

namespace Fge
{

class NewSceneSetEvent : public Event
{
};

class SceneManager
{
public:
  void on_fixed_update(float frametime);

  void on_update(float delta_time);

  void on_render();

  void set_scene(std::shared_ptr<Scene> scene);

  void terminate();

private:
  std::shared_ptr<Scene> scene{};
};

} // namespace Fge

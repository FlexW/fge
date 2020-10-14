#include "scene_manager.hpp"
#include "application.hpp"

namespace Fge
{

void SceneManager::on_fixed_update(float frametime)
{
  if (!scene)
  {
    return;
  }

  scene->on_fixed_update(frametime);
}

void SceneManager::on_update(float delta_time)
{
  if (!scene)
  {
    return;
  }

  scene->on_update(delta_time);
}

void SceneManager::on_render()
{
  if (!scene)
  {
    return;
  }

  scene->on_render();
}

void SceneManager::set_scene(std::shared_ptr<Scene> scene)
{
  if (this->scene != nullptr)
  {
    this->scene->on_destroy();
  }

  this->scene = scene;

  auto app = Application::get_instance();

  NewSceneSetEvent new_scene_set_event{};
  app->get_event_manager()->publish(&new_scene_set_event);

  scene->on_create();
}

} // namespace Fge

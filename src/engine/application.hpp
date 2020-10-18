#pragma once

#include "config/config_manager.hpp"
#include "event/event_manager.hpp"
#include "file/file_manager.hpp"
#include "graphic/graphic_manager.hpp"
#include "layer.hpp"
#include "layer_stack.hpp"
#include "physic/physic_manager.hpp"
#include "resources/resource_manager.hpp"
#include "scene/scene_manager.hpp"
#include "std.hpp"

namespace Fge
{

class Application
{
public:
  ~Application();

  static std::shared_ptr<Application> get_instance();

  void init_application(int argc, char *argv[]);

  void terminate_application();

  int run();

  void push_layer(std::unique_ptr<Layer> layer);

  std::shared_ptr<EventManager> get_event_manager() { return event_manager; }

  std::shared_ptr<ConfigManager> get_config_manager() { return config_manager; }

  std::shared_ptr<FileManger> get_file_manager() { return file_system_manager; }

  std::shared_ptr<ResourceManager> get_resource_manager()
  {
    return resource_manager;
  }

  std::shared_ptr<GraphicManager> get_graphic_manager()
  {
    return graphic_manager;
  }

  std::shared_ptr<SceneManager> get_scene_manager() { return scene_manager; }

  std::shared_ptr<PhysicManager> get_physic_manager() { return physic_manager; }

  void close();

  float get_delta_time() const { return delta_time; }

private:
  static std::once_flag               instance_created;
  static std::shared_ptr<Application> instance;

  LayerStack layer_stack;

  std::shared_ptr<EventManager>    event_manager{};
  std::shared_ptr<ConfigManager>   config_manager{};
  std::shared_ptr<FileManger>      file_system_manager{};
  std::shared_ptr<ResourceManager> resource_manager{};
  std::shared_ptr<GraphicManager>  graphic_manager{};
  std::shared_ptr<SceneManager>    scene_manager{};
  std::shared_ptr<PhysicManager>   physic_manager{};

  bool close_app = false;

  float delta_time = 0.0f;

  Application();

  Application(const Application &other) = default;

  Application &operator=(const Application &other) = default;

  bool on_close(const WindowCloseEvent *const event);

  void main_loop();

  void init_logging();
};

} // namespace Fge

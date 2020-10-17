#include "application.hpp"
#include "GLFW/glfw3.h"
#include "file/root_directory.hpp"
#include "log/io_log_sink.hpp"
#include "log/log.hpp"
#include "scene/scene_manager.hpp"
#include "util/time.hpp"

namespace Fge
{

std::once_flag               Application::instance_created{};
std::shared_ptr<Application> Application::instance{};

Application::Application() {}

Application::~Application() {}

std::shared_ptr<Application> Application::get_instance()
{
  std::call_once(instance_created, []() {
    instance = std::shared_ptr<Application>(new Application());
  });

  return instance;
}

void Application::init_logging()
{
  const std::string log_level_req =
      config_manager->get_config()["game"]["log_level"];
  LogType log_level = LogType::Debug;

  if (log_level_req == "Debug")
  {
    log_level = LogType::Debug;
  }
  else if (log_level_req == "Info")
  {
    log_level = LogType::Info;
  }
  else if (log_level_req == "Warning")
  {
    log_level = LogType::Warning;
  }
  else if (log_level_req == "Error")
  {
    log_level = LogType::Error;
  }
  else if (log_level_req == "Trace")
  {
    log_level = LogType::Trace;
  }

  const std::string log_mode_req =
      config_manager->get_config()["game"]["log_mode"];
  LogMode log_mode = LogMode::Async;

  if (log_mode_req == "ASYNC")
  {
    log_mode = LogMode::Async;
  }
  else if (log_mode_req == "SYNC")
  {
    log_mode = LogMode::Sync;
  }

  start_logger<IOLogSink>(log_level, log_mode);
}

void Application::init_application(int /*argc*/, char ** /*argv*/)
{
  try
  {
    // Create managers, order is important as some managers depend on each other

    // Create event manager
    event_manager = std::make_shared<EventManager>();

    // Create file manager
    file_system_manager = std::make_shared<FileManger>(root_directory);

    // Create Config manager
    config_manager = std::make_shared<ConfigManager>();

    // Logging system can now setted up
    init_logging();

    // Create resource manager
    resource_manager = std::make_shared<ResourceManager>();

    // Create graphic manager
    graphic_manager = std::make_shared<GraphicManager>();

    // Create scene manager
    scene_manager = std::make_shared<SceneManager>();

    // Register for events
    event_manager->subscribe(this, &Application::on_close);
  }
  catch (std::exception &e)
  {
    error("Application", "Unhandled exception while init: {}", e.what());
    std::exit(EXIT_FAILURE);
  }
}

bool Application::on_close(const WindowCloseEvent *const /*event*/)
{
  close_app = true;

  return false;
}

void Application::terminate_application()
{
  scene_manager->terminate();
  graphic_manager->terminate();
  terminate_logger();
}

int Application::run()
{
  try
  {
    graphic_manager->create_window();
  }
  catch (std::exception &e)
  {
    error("Application", "Could not init graphic manager: {}", e.what());
    std::exit(EXIT_FAILURE);
  }

  try
  {
    layer_stack.on_init();
  }
  catch (std::exception &e)
  {
    error("Application", "Could not init layers: {}", e.what());
    std::exit(EXIT_FAILURE);
  }

  try
  {
    this->main_loop();
  }
  catch (std::exception &error)
  {
    std::cout << "Uncatched exception while running main loop: " << error.what()
              << std::endl;
    std::exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}

void Application::main_loop()
{
  double last_time = get_current_time_millis() / 1000.0;

  while (!close_app)
  {
    double current_time = double(get_current_time_millis()) / 1000.0;
    delta_time          = static_cast<float>(current_time - last_time);

    scene_manager->on_update(delta_time);
    layer_stack.on_update(delta_time);

    graphic_manager->begin_render();

    layer_stack.on_render();

    graphic_manager->begin_imgui_render();
    layer_stack.on_imgui_render();
    graphic_manager->end_imgui_render();

    graphic_manager->end_render();

    graphic_manager->flush();

    last_time = current_time;
  }

  terminate_application();
}

void Application::push_layer(std::unique_ptr<Layer> layer)
{
  layer_stack.push_layer(std::move(layer));
}

void Application::close() { close_app = true; }

} // namespace Fge

#include "application.hpp"
#include "log/io_log_sink.hpp"
#include "log/log.hpp"
#include "scene/scene_manager.hpp"

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
  // Create managers, order is important as some managers depend on each other

  // Create event manager
  event_manager = std::make_shared<EventManager>();

  // Create file manager
  const auto app_root_path = std::filesystem::current_path();
  file_system_manager      = std::make_shared<FileManger>(app_root_path);

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

bool Application::on_close(const WindowCloseEvent *const /*event*/)
{
  close_app = true;

  return false;
}

void Application::terminate_application()
{
  graphic_manager->terminate();
  terminate_logger();
}

int Application::run()
{
  graphic_manager->create_window();

  layer_stack.on_init();

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

  return 0;
}

void Application::main_loop()
{
  while (!close_app)
  {

    graphic_manager->begin_render();

    layer_stack.on_render();

    graphic_manager->begin_imgui_render();
    layer_stack.on_imgui_render();
    graphic_manager->end_imgui_render();

    graphic_manager->end_render();

    graphic_manager->flush();
  }

  terminate_application();
}

void Application::push_layer(std::unique_ptr<Layer> layer)
{
  layer_stack.push_layer(std::move(layer));
}

void Application::close() { close_app = true; }

} // namespace Fge

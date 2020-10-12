#include "config_manager.hpp"
#include "application.hpp"
#include "util/assert.hpp"

#define CONFIG_FILE_NAME "config.lua"

namespace Fge
{

ConfigManager::ConfigManager()
{
  auto app              = Application::get_instance();
  auto config_file_path = app->get_file_manager()->get_app_config_path();
  config_file_path      = config_file_path / CONFIG_FILE_NAME;

  lua.open_libraries(sol::lib::base);

  // Load and execute config file
  auto result = lua.script_file(config_file_path.string());

  if (!result.valid())
  {
    throw std::runtime_error("Error processing configuration file");
  }
}

} // namespace Fge

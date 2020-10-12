#include "resource_manager.hpp"
#include "application.hpp"

namespace Fge
{

ResourceManager::ResourceManager()
{
  auto app                = Application::get_instance();
  auto filesystem_manager = app->get_file_manager();
  resource_path           = filesystem_manager->get_app_res_path();
}

} // namespace Fge

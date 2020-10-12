#include "file_manager.hpp"

#define CONFIG_DIR "config"
#define RES_DIR    "res"
#define CACHE_DIR  ".cache"

namespace Fge
{

FileManger::FileManger(const std::filesystem::path &app_root_path)
    : app_root_path(app_root_path)
{
  app_config_path = app_root_path / CONFIG_DIR;
  app_res_path    = app_res_path / RES_DIR;
  app_cache_path  = app_root_path / CACHE_DIR;

  std::filesystem::create_directory(app_cache_path);
}

} // namespace Fge

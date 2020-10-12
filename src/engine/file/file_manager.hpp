#pragma once

#include "std.hpp"

namespace Fge
{

class FileManger
{
public:
  FileManger(const std::filesystem::path &app_root_path);

  const std::filesystem::path &get_app_root_path() const
  {
    return app_root_path;
  }

  const std::filesystem::path &get_app_config_path() const
  {
    return app_config_path;
  }

  const std::filesystem::path &get_app_res_path() const { return app_res_path; }

  const std::filesystem::path &get_app_cache_path() const
  {
    return app_cache_path;
  }

private:
  std::filesystem::path app_root_path;
  std::filesystem::path app_config_path;
  std::filesystem::path app_res_path;
  std::filesystem::path app_cache_path;
};

} // namespace Fge

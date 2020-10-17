#pragma once

#include "std.hpp"
#include <filesystem>

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

  const std::filesystem::path &get_shaders_path() const
  {
    return app_shader_path;
  }

  const std::filesystem::path &get_meshes_path() const
  {
    return app_meshes_path;
  }

  const std::filesystem::path &get_textures_path() const
  {
    return app_textures_path;
  }

  const std::filesystem::path &get_scripts_path() const
  {
    return app_scripts_path;
  }

  std::string read_text_file(const std::string &filepath);

private:
  std::filesystem::path app_root_path;
  std::filesystem::path app_config_path;
  std::filesystem::path app_res_path;
  std::filesystem::path app_cache_path;
  std::filesystem::path app_shader_path;
  std::filesystem::path app_meshes_path;
  std::filesystem::path app_textures_path;
  std::filesystem::path app_scripts_path;
};

} // namespace Fge

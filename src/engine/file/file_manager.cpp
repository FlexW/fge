#include "file_manager.hpp"
#include <stdexcept>

#define CONFIG_DIR   "config"
#define RES_DIR      "res"
#define CACHE_DIR    ".cache"
#define SHADER_DIR   "shaders"
#define MESHES_DIR   "meshes"
#define TEXTURES_DIR "textures"

namespace Fge
{

FileManger::FileManger(const std::filesystem::path &app_root_path)
    : app_root_path(app_root_path)
{
  app_config_path = app_root_path / CONFIG_DIR;
  app_res_path    = app_res_path / RES_DIR;
  app_cache_path  = app_root_path / CACHE_DIR;

  app_shader_path   = app_res_path / SHADER_DIR;
  app_meshes_path   = app_res_path / MESHES_DIR;
  app_textures_path = app_res_path / TEXTURES_DIR;

  std::filesystem::create_directory(app_cache_path);
}

std::string FileManger::read_text_file(const std::string &filepath)
{
  std::ifstream in(filepath);

  if (!in.is_open())
  {
    throw std::runtime_error("Could not open file: " + filepath);
  }

  std::string contents((std::istreambuf_iterator<char>(in)),
                       std::istreambuf_iterator<char>());

  return contents;
}

} // namespace Fge

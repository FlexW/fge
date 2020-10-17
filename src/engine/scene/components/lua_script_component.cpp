#include "lua_script_component.hpp"
#include "application.hpp"
#include "log/log.hpp"
#include "scene/component.hpp"

namespace Fge
{
LuaScriptComponent::LuaScriptComponent(Actor *            owner,
                                       int                update_order,
                                       const std::string &type_name)
    : Component(owner, update_order, type_name)
{
}

void LuaScriptComponent::set_script_from_file(const std::string &filepath)
{
  script_filepath = filepath;

  load_script();
}

void LuaScriptComponent::create()
{
  created = true;

  load_script();
}

void LuaScriptComponent::update(float delta_time)
{
  if (!script_loaded)
  {
    return;
  }

  lua["update"](delta_time);
}

void LuaScriptComponent::load_script()
{
  if (script_filepath == "" || !created)
  {
    return;
  }

  trace("LuaScriptComponent", "Load script: {}", script_filepath);

  auto app          = Application::get_instance();
  auto file_manager = app->get_file_manager();

  lua.open_libraries(sol::lib::base);
  lua.script_file(file_manager->get_scripts_path() / script_filepath);

  script_loaded = true;
}

} // namespace Fge

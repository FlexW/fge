#include "lua_script_component.hpp"
#include "application.hpp"
#include "glm/ext/vector_float3.hpp"
#include "log/log.hpp"
#include "scene/actor.hpp"
#include "scene/component.hpp"
#include "util/time.hpp"

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

  auto result = lua["update"](delta_time);
  if (!result.valid())
  {
    sol::error e = result;
    warning("LuaScriptComponent", "Execution of update() failed: {}", e.what());
  }
}

void LuaScriptComponent::lua_bind_util(sol::state &lua)
{
  lua["get_current_time_millis"] = get_current_time_millis;
}

void LuaScriptComponent::lua_bind_math(sol::state &lua)
{
  auto mult_overloads = sol::overload(
      [](const glm::vec3 &v1, const glm::vec3 &v2) -> glm::vec3 {
        return v1 * v2;
      },
      [](const glm::vec3 &v1, float f) -> glm::vec3 { return v1 * f; },
      [](float f, const glm::vec3 &v1) -> glm::vec3 { return f * v1; });

  auto add_overloads = sol::overload(
      [](const glm::vec3 &v1, const glm::vec3 &v2) -> glm::vec3 {
        return v1 + v2;
      },
      [](const glm::vec3 &v1, float f) -> glm::vec3 { return v1 + f; },
      [](float f, const glm::vec3 &v1) -> glm::vec3 { return f + v1; });

  auto vec3_type = lua.new_usertype<glm::vec3>(
      "vec3",
      sol::constructors<glm::vec3(),
                        glm::vec3(float),
                        glm::vec3(float, float, float)>(),
      "x",
      &glm::vec3::x,
      "y",
      &glm::vec3::y,
      "z",
      &glm::vec3::z,
      sol::meta_function::addition,
      add_overloads,
      sol::meta_function::multiplication,
      mult_overloads,
      sol::meta_function::to_string,
      [](const glm::vec3 &v) { return glm::to_string(v); });

  lua["sin"] = [](float v) { return glm::sin(v); };
  lua["cos"] = [](float v) { return glm::cos(v); };
  lua["tan"] = [](float v) { return glm::tan(v); };
  lua["abs"] = glm::abs<float>;
}

void LuaScriptComponent::lua_bind_scene(sol::state &lua)
{
   auto actor_type = lua.new_usertype<Actor>("Actor",
                                            sol::no_constructor,
                                            "set_position",
                                            &Actor::set_position,
                                            "set_rotation",
                                            &Actor::set_rotation_euler);
  lua["owner"]    = owner;
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

  lua_bind_util(lua);
  lua_bind_math(lua);
  lua_bind_scene(lua);

  try
  {
    lua.script_file(
        (file_manager->get_scripts_path() / script_filepath).string(),
        sol::script_default_on_error);
  }
  catch (std::exception &e)
  {
    warning("LuaScriptComponent", "Script failed: {}", e.what());
  }

  script_loaded = true;
}

} // namespace Fge

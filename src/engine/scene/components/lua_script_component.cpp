#include "lua_script_component.hpp"
#include "application.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/ext/vector_float3.hpp"
#include "graphic/graphic_manager.hpp"
#include "graphic/window.hpp"
#include "log/log.hpp"
#include "scene/actor.hpp"
#include "scene/component.hpp"
#include "scene/components/skinned_mesh_component.hpp"
#include "sol/raii.hpp"
#include "sol/types.hpp"
#include "util/time.hpp"
#include <memory>

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
  auto vec3_mult_overloads = sol::overload(
      [](const glm::vec3 &v1, const glm::vec3 &v2) -> glm::vec3 {
        return v1 * v2;
      },
      [](const glm::vec3 &v1, float f) -> glm::vec3 { return v1 * f; },
      [](float f, const glm::vec3 &v1) -> glm::vec3 { return f * v1; });

  auto vec3_add_overloads = sol::overload(
      [](const glm::vec3 &v1, const glm::vec3 &v2) -> glm::vec3 {
        return v1 + v2;
      },
      [](const glm::vec3 &v1, float f) -> glm::vec3 { return v1 + f; },
      [](float f, const glm::vec3 &v1) -> glm::vec3 { return f + v1; });

  auto vec3_sub_overloads = sol::overload(
      [](const glm::vec3 &v1, const glm::vec3 &v2) -> glm::vec3 {
        return v1 - v2;
      },
      [](const glm::vec3 &v1, float f) -> glm::vec3 { return v1 - f; },
      [](float f, const glm::vec3 &v1) -> glm::vec3 { return f - v1; });

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
      vec3_add_overloads,
      sol::meta_function::subtraction,
      vec3_sub_overloads,
      sol::meta_function::multiplication,
      vec3_mult_overloads,
      sol::meta_function::to_string,
      [](const glm::vec3 &v) { return glm::to_string(v); });

  auto quat_mult_overloads =
      sol::overload([](const glm::quat &q1, const glm::quat &q2) -> glm::quat {
        return q1 * q2;
      });

  auto quat_type = lua.new_usertype<glm::quat>(
      "quat",
      sol::constructors<glm::quat(), glm::quat(float, float, float, float)>(),
      sol::meta_function::multiplication,
      quat_mult_overloads);

  lua["sin"] = [](double v) { return glm::sin(v); };
  lua["cos"] = [](double v) { return glm::cos(v); };
  lua["tan"] = [](double v) { return glm::tan(v); };
  lua["abs"] = glm::abs<double>;

  lua["angle_axis"] = [](float angle, const glm::vec3 &axis) -> glm::quat {
    return glm::angleAxis(angle, axis);
  };
}

template <typename TComponent>
std::shared_ptr<TComponent> cast_component(std::shared_ptr<Component> component)
{
  return std::dynamic_pointer_cast<TComponent>(component);
}

void LuaScriptComponent::lua_bind_scene(sol::state &lua)
{
  auto app_type =
      lua.new_usertype<Application>("Application",
                                    sol::no_constructor,
                                    "get_graphic_manager",
                                    &Application::get_graphic_manager);

  lua["app"] = Application::get_instance();

  auto graphic_manager_type =
      lua.new_usertype<GraphicManager>("GraphicManager",
                                       sol::no_constructor,
                                       "get_window",
                                       &GraphicManager::get_window);

  auto window_type = lua.new_usertype<Window>("Window",
                                              sol::no_constructor,
                                              "get_key",
                                              &Window::get_key,
                                              "get_mouse_offset_x",
                                              &Window::get_mouse_offset_x,
                                              "get_mouse_offset_y",
                                              &Window::get_mouse_offset_y);

  auto key_type =
      lua.new_enum("Key", "W", Key::W, "S", Key::S, "A", Key::A, "D", Key::D);

  auto key_action_type = lua.new_enum("KeyAction",
                                      "Press",
                                      KeyAction::Press,
                                      "Release",
                                      KeyAction::Release,
                                      "Repeat",
                                      KeyAction::Repeat,
                                      "Unknown",
                                      KeyAction::Unknown);

  auto actor_type =
      lua.new_usertype<Actor>("Actor",
                              sol::no_constructor,
                              "set_position",
                              &Actor::set_position,
                              "get_position",
                              &Actor::get_position,
                              "set_rotation_euler",
                              &Actor::set_rotation_euler,
                              "get_rotation_euler",
                              &Actor::get_rotation_euler,
                              "set_rotation",
                              &Actor::set_rotation,
                              "get_rotation",
                              &Actor::get_rotation,
                              "set_scale",
                              &Actor::set_scale,
                              "get_forward",
                              &Actor::get_forward,
                              "get_right",
                              &Actor::get_right,
                              "find_component_by_type_name",
                              &Actor::find_component_by_type_name);

  lua.new_usertype<Component>("Component",
                              sol::no_constructor,
                              "type_name",
                              &Component::type_name);

  lua.new_usertype<SkinnedMeshComponent>(
      "SkinnedMeshComponent",
      sol::no_constructor,
      sol::base_classes,
      sol::bases<Component>(),
      "stop_current_animation",
      &SkinnedMeshComponent::stop_current_animation,
      "play_animation_endless",
      &SkinnedMeshComponent::play_animation_endless);
  lua["cast_skinned_mesh_component"] = cast_component<SkinnedMeshComponent>;

  lua["owner"] = owner;
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

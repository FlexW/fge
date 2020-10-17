#pragma once

#include "platform/lua/lua.hpp"
#include "scene/component.hpp"

namespace Fge
{

class LuaScriptComponent : public Component
{
public:
  LuaScriptComponent(Actor *            owner,
                     int                update_order = 50,
                     const std::string &type_name = "Fge::LuaScriptComponent");

  void set_script_from_file(const std::string &filepath);

protected:
  void create() override;

  void update(float delta_time) override;

private:
  std::string script_filepath{};

  sol::state lua;

  bool created = false;

  bool script_loaded = false;

  void load_script();
};

} // namespace Fge

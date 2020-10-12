#pragma once

#include "platform/lua/lua.hpp"

namespace Fge
{

class ConfigManager
{
public:
  ConfigManager();

  sol::state &get_config() { return lua; }

private:
  sol::state lua;
};

} // namespace Fge

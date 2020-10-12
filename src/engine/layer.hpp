#pragma once

#include "std.hpp"

namespace Fge
{

class Layer
{
public:
  virtual ~Layer() = default;

  virtual void init() = 0;

  virtual void update(float delta_time) = 0;

  virtual void fixed_update(float frametime) = 0;

  virtual void render() = 0;

  virtual void imgui_render() = 0;
};

} // namespace Fge

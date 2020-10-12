#pragma once

#include "layer.hpp"
namespace Fge
{

class EditorLayer : public Layer
{
  virtual void init();

  virtual void update(float delta_time);

  virtual void fixed_update(float frametime);

  virtual void render();
};

} // namespace Fge

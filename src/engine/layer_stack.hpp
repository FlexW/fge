#pragma once

#include "layer.hpp"
#include "std.hpp"

namespace Fge
{

class LayerStack
{
public:
  void push_layer(std::unique_ptr<Layer> layer);

  void on_init();

  void on_update(float delta_time);

  void on_fixed_update(float frametime);

  void on_render();

  void on_imgui_render();

private:
  std::vector<std::unique_ptr<Layer>> layers;
};

} // namespace Fge

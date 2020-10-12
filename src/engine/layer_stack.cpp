#include "layer_stack.hpp"

namespace Fge
{

void LayerStack::push_layer(std::unique_ptr<Layer> layer)
{
  layers.emplace_back(std::move(layer));
}

void LayerStack::on_init()
{
  for (auto &layer : layers)
  {
    layer->init();
  }
}

void LayerStack::on_update(float delta_time)
{
  for (auto &layer : layers)
  {
    layer->update(delta_time);
  }
}

void LayerStack::on_fixed_update(float frametime)
{
  for (auto &layer : layers)
  {
    layer->fixed_update(frametime);
  }
}

void LayerStack::on_render()
{
  for (auto &layer : layers)
  {
    layer->render();
  }
}

} // namespace Fge

#include "layer_stack.hpp"

namespace fge
{

void layer_stack::init()
{
  for (auto &layer : layers)
  {
    layer->init();
  }
}

void layer_stack::update()
{
  for (auto &layer : layers)
  {
    layer->update();
  }
}

void layer_stack::terminate()
{
  for (auto &layer : layers)
  {
    layer->terminate();
  }
}

} // namespace fge

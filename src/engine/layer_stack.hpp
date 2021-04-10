#pragma once

#include <memory>
#include <vector>

#include "layer.hpp"

namespace fge
{

class layer_stack
{
public:
  template <typename TLayer> void add_layer(application *app)
  {
    layers.emplace_back(new TLayer(app));
  }

  void init();

  void update(float delta_time);

  void terminate();

private:
  std::vector<std::unique_ptr<layer>> layers;
};

} // namespace fge

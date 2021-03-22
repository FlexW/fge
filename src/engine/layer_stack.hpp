#pragma once

#include <memory>
#include <vector>

#include "layer.hpp"

namespace fge
{

class layer_stack
{
public:
  template <typename TLayer> void add_layer()
  {
    layers.emplace_back(new TLayer);
  }

  void init();

  void update();

  void terminate();

private:
  std::vector<std::unique_ptr<layer>> layers;
};

} // namespace fge

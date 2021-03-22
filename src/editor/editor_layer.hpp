#pragma once

#include "layer.hpp"

namespace fge
{

class editor_layer : public layer
{
public:
  void init() override;

  void update() override;

  void terminate() override;
};

} // namespace fge

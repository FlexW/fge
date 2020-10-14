#pragma once

#include "std.hpp"

namespace Fge
{

class Texture2D
{
public:
  virtual ~Texture2D() = default;

  virtual void bind(uint32_t slot) = 0;
};

} // namespace Fge

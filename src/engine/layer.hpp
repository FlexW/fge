#pragma once

namespace fge
{

class layer
{
public:
  virtual void init() = 0;

  virtual void update() = 0;

  virtual void terminate() = 0;
};

} // namespace fge

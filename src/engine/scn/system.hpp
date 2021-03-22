#pragma once

namespace fge::scn
{

class system
{
public:
  virtual void create() = 0;

  virtual void update() = 0;

  virtual void destroy() = 0;
};

} // namespace fge::scn

#pragma once

namespace Fge::Gl
{

class GlWindow
{

public:
  virtual ~GlWindow() = default;

  virtual void init_open_gl() = 0;
};

} // namespace Fge::Gl

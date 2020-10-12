#pragma once

#include "std.hpp"
#include "window.hpp"

namespace Fge
{

class GraphicManager
{
public:
  void create_window();

  void flush();

  std::shared_ptr<Window> get_window() { return window; }

private:
  std::shared_ptr<Window> window{};
};

} // namespace Fge

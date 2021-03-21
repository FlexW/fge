#pragma once

#include <bits/stdint-uintn.h>
#include <memory>
#include <string>

#include "broadcast.hpp"
#include "gfx/window.hpp"

namespace fge::gfx
{

struct window_create_info
{
  std::string window_title = "fge";
  uint32_t    window_width = 800;
  uint32_t    window_height = 600;
  uint8_t     samples       = 0;
  bool        debug         = true;
  bool        fullscreen    = false;
  bool        vsync         = false;
};

struct graphic_backend_context
{
  std::unique_ptr<window> (*create_window)(broadcast *application_broadcast,
                                           const window_create_info &);
};

} // namespace fge::gfx

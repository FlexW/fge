#pragma once

#include <functional>
#include <memory>
#include <string>

#include "gfx/window.hpp"
#include "graphic_backend_context.hpp"

namespace fge::gfx::graphic_backend
{

void init();

std::unique_ptr<window> create_window(broadcast *application_broadcast,
                                      const window_create_info &create_info);

} // namespace fge::gfx::graphic_backend

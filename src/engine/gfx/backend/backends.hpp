#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "opengl_backend.hpp"

namespace fge::gfx
{

inline const std::unordered_map<std::string,
                                std::function<graphic_backend_context(void)>>
    graphic_backends = {
        {"opengl", gl::load_opengl_backend_context},
};

} // namespace fge::gfx

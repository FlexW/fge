#pragma once

#include "graphic_backend.hpp"

namespace fge::gfx::gl
{

vertex_buffer create_vertex_buffer(const memory &              memory,
                                   const vertex_buffer_layout &layout);

index_buffer create_index_buffer(const memory &memory);
}

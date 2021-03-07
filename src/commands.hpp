#pragma once

#include <string>

#include "graphic_backend_types.hpp"
#include <graphic_backend.hpp>

namespace fge::gfx
{

using dispatch_function = void (*)(const void *);

struct draw_command
{
  static constexpr dispatch_function DISPATCH_FUNCTION = &draw;

  vertex_buffer_handle  vertices;
  shader_program_handle shader;

  bool depth_test;
};
static_assert(std::is_pod<draw_command>::value == true,
              "draw_command must be a POD.");

template <typename TValue> struct set_uniform_command
{
  static constexpr dispatch_function DISPATCH_FUNCTION = &set_uniform;

  shader_program_handle shader;
  uniform_type          type;
  const char *          name;
  TValue                data;
};

} // namespace fge::gfx

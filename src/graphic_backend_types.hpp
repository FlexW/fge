#pragma once

#include <cstddef>
#include <cstdint>

#include "util.hpp"

namespace fge::gfx
{

using vertex_buffer_layout_handle = uint32_t;
using vertex_buffer_handle        = uint32_t;
using index_buffer_handle         = uint32_t;
using shader_program_handle       = uint32_t;
using texture_handle              = uint32_t;
using uniform_handle              = uint32_t;

struct memory
{
  void *      data{};
  std::size_t size;
};

enum class attribute_type
{
  floating
};

struct index_buffer
{
  index_buffer_handle handle{};
};

enum class render_state : uint8_t
{
  no_state = 0x0,

  clear_color = 0x1,
  clear_depth = 0x2
};
FGE_ENABLE_BITMASK_OPERATORS(render_state)

enum class wrap_mode
{
  repeat,
  clamp_to_edge
};

enum class texture_type
{
  two_d
};

enum class filter_mode
{
  linear
};

enum class pixel_internal_format
{
  rgba,
  rgb,
  alpha
};

enum class pixel_format
{
  rgb,
  rgba
};

enum class pixel_type
{
  ubyte
};

struct texture_info
{
  texture_type type;

  uint32_t width;
  uint32_t height;

  wrap_mode wrap_u;
  wrap_mode wrap_v;

  filter_mode minification_filter;
  filter_mode magnification_filter;

  pixel_internal_format internal_format;

  pixel_format source_format;
  pixel_type   source_type;

  bool generate_mipmap;
};

enum class uniform_type
{
  mat4,
  texture2d
};

} // namespace fge::gfx

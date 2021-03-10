#pragma once

#include <cstddef>
#include <cstdint>

#include "util.hpp"

namespace fge::gfx
{

using handle_t                    = uint32_t;
using vertex_buffer_layout_handle = handle_t;
using vertex_buffer_handle        = handle_t;
using index_buffer_handle         = handle_t;
using shader_program_handle       = handle_t;
using texture_handle              = handle_t;
using uniform_handle              = handle_t;
using framebuffer_handle          = handle_t;
using render_target_handle        = handle_t;
using renderbuffer_handle         = handle_t;

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
  clamp_to_edge,
  clamp_to_border
};

enum class texture_type
{
  two_d
};

enum class filter_mode
{
  linear,
  nearest,
  linear_mipmap_linear
};

enum class pixel_internal_format
{
  rgba,
  rgb,
  alpha,
  depth
};

enum class pixel_format
{
  rgb,
  rgba,
  depth
};

enum class pixel_type
{
  ubyte,
  floating
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

  bool  enable_border_color = false;
  float border_color[4];
};

struct renderbuffer_info
{
  uint32_t width;
  uint32_t height;

  pixel_internal_format internal_format;
};

enum class uniform_type
{
  mat4,
  vec3,
  texture2d
};

enum class render_target
{
  renderbuffer,
  texture
};

enum class attachment_type
{
  color,
  depth,
  stencil,
  depth_stencil
};

struct attachment_info
{
  attachment_type      type;
  render_target        target_type;
  render_target_handle handle;
};

} // namespace fge::gfx

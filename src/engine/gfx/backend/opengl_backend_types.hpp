#pragma once

#include <cstddef>

#include "opengl.hpp"

namespace fge::gfx::gl
{

struct vertex_buffer
{
  GLuint      id;
  GLuint      vao;
  std::size_t size;
};

struct texture
{
  GLuint id;
  GLint  type;
};

struct shader
{
  GLuint id;
  GLuint next_free_texture_slot = 0;
};

struct framebuffer
{
  GLuint id;
};

struct renderbuffer
{
  GLuint id;
};

struct gl_type_info
{
  GLuint      type;
  std::size_t size;
};

enum class shader_type
{
  vertex,
  fragment
};

} // namespace fge::gfx::gl

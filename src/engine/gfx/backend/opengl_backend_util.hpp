#pragma once

#include "gfx/graphic_backend_types.hpp"
#include "opengl.hpp"
#include "opengl_backend_types.hpp"
#include "util/assert.hpp"

namespace fge::gfx::gl
{

GLenum to_gl(wrap_mode mode);

GLenum to_gl(filter_mode mode);

GLint to_gl(pixel_internal_format format);

GLint to_gl(pixel_format format);

GLint to_gl(pixel_type type);

GLint to_gl(texture_type type);

gl_type_info get_gl_type_info(attribute_type type);

} // namespace fge::gfx::gl

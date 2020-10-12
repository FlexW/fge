#pragma once

#include "gl.hpp"

namespace Fge::Gl
{

void dump_info();

void APIENTRY debug_callback(GLenum        source,
                             GLenum        type,
                             GLuint        id,
                             GLenum        severity,
                             GLsizei       length,
                             const GLchar *msg,
                             const void *  param);

} // namespace Fge::Gl

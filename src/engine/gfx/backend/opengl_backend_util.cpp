#include "opengl_backend_util.hpp"

namespace fge::gfx::gl
{

GLenum to_gl(wrap_mode mode)
{
  switch (mode)
  {
  case wrap_mode::clamp_to_edge:
    return GL_CLAMP_TO_EDGE;
  case wrap_mode::repeat:
    return GL_REPEAT;
  case wrap_mode::clamp_to_border:
    return GL_CLAMP_TO_BORDER;
  default:
    FGE_FAIL("Can not handle wrap mode");
  }
}

GLenum to_gl(filter_mode mode)
{
  switch (mode)
  {
  case filter_mode::linear:
    return GL_LINEAR;
  case filter_mode::linear_mipmap_linear:
    return GL_LINEAR_MIPMAP_LINEAR;
  case filter_mode::nearest:
    return GL_NEAREST;
  default:
    FGE_FAIL("Can not handle filter mode");
  }
}

GLint to_gl(pixel_internal_format format)
{
  switch (format)
  {
  case pixel_internal_format::rgb:
    return GL_RGB;
  case pixel_internal_format::depth:
    return GL_DEPTH_COMPONENT;
  default:
    FGE_FAIL("Can not handle pixel internal format");
  }
}

GLint to_gl(pixel_format format)
{
  switch (format)
  {
  case pixel_format::rgb:
    return GL_RGB;
  case pixel_format::rgba:
    return GL_RGBA;
  case pixel_format::depth:
    return GL_DEPTH_COMPONENT;
  default:
    FGE_FAIL("Can not handle pixel format");
  }
}

GLint to_gl(pixel_type type)
{
  switch (type)
  {
  case pixel_type::ubyte:
    return GL_UNSIGNED_BYTE;
  case pixel_type::floating:
    return GL_FLOAT;
  default:
    FGE_FAIL("Can not handle pixel type");
  }
}

GLint to_gl(texture_type type)
{
  switch (type)
  {
  case texture_type::two_d:
    return GL_TEXTURE_2D;
  default:
    FGE_FAIL("Can not handle texture type");
  }
}

gl_type_info get_gl_type_info(attribute_type type)
{
  std::size_t real_type_size;
  GLuint      real_type;

  switch (type)
  {
  case attribute_type::floating:
    real_type      = GL_FLOAT;
    real_type_size = sizeof(GLfloat);
    break;
  default:
    FGE_FAIL("Can not handle type");
  }

  gl_type_info info{};
  info.type = real_type;
  info.size = real_type_size;

  return info;
}
}

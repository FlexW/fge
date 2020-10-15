#include "texture2d.hpp"
#include "graphic/texture.hpp"
#include "log/log.hpp"
#include "util/assert.hpp"

namespace Fge::Gl
{

GLenum format_to_gl_format(ImageFormat format)
{
  switch (format)
  {
  case ImageFormat::Red:
    return GL_RED;

  case ImageFormat::Rgb:
    return GL_RGB;

  case ImageFormat::Rgba:
    return GL_RGBA;

  default:
    FGE_FAIL("Can not handle format");
  }
}

GLenum pixel_type_to_gl_pixel_type(PixelDataType data_type)
{
  switch (data_type)
  {
  case PixelDataType::UnsignedByte:
    return GL_UNSIGNED_BYTE;

  default:
    FGE_FAIL("Can not handle data type");
  }
}

GLint wrap_mode_to_gl_wrap_mode(WrapMode wrap_mode)
{
  switch (wrap_mode)
  {
  case WrapMode::Repeat:
    return GL_REPEAT;

  default:
    FGE_FAIL("Can not handle wrap mode");
  }
}

GLint filter_mode_to_gl_filter_mode(FilterMode filter_mode)
{
  switch (filter_mode)
  {
  case FilterMode::Linear:
    return GL_LINEAR;

  case FilterMode::LinearMipmapLinear:
    return GL_LINEAR_MIPMAP_LINEAR;

  default:
    FGE_FAIL("Can not handle filter mode");
  }
}

Texture2D::Texture2D(const Texture2DConfig &config)
{
  if (config.samples > 0)
  {
    target = GL_TEXTURE_2D_MULTISAMPLE;
  }

  glGenTextures(1, &id);
  trace("Texture2D", "Created texture2d with id: {}", id);

  glBindTexture(target, id);

  switch (target)
  {
  case GL_TEXTURE_2D:
  {
    auto gl_format = format_to_gl_format(config.format);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 gl_format,
                 config.width,
                 config.height,
                 0,
                 gl_format,
                 pixel_type_to_gl_pixel_type(config.pixel_data_type),
                 config.data);
    break;
  }

  case GL_TEXTURE_2D_MULTISAMPLE:
  {
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
                            config.samples,
                            format_to_gl_format(config.format),
                            config.width,
                            config.height,
                            GL_TRUE);

    break;
  }

  default:
    FGE_FAIL("Can not handle texture target");
  }

  if (config.generate_mipmap)
  {
    glGenerateMipmap(target);
  }

  // Set texture wrap and filter modes
  if (target == GL_TEXTURE_2D)
  {
    glTexParameteri(target,
                    GL_TEXTURE_WRAP_S,
                    wrap_mode_to_gl_wrap_mode(config.wrap_s));
    glTexParameteri(target,
                    GL_TEXTURE_WRAP_T,
                    wrap_mode_to_gl_wrap_mode(config.wrap_t));
    glTexParameteri(target,
                    GL_TEXTURE_MIN_FILTER,
                    filter_mode_to_gl_filter_mode(config.filter_min));
    glTexParameteri(target,
                    GL_TEXTURE_MAG_FILTER,
                    filter_mode_to_gl_filter_mode(config.filter_max));
  }

  glBindTexture(target, 0);
}

Texture2D::~Texture2D()
{
  trace("Texture2D", "Delete texture2d with id: {}", id);
  glDeleteTextures(1, &id);
}

void Texture2D::bind(uint32_t slot)
{
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(target, id);
}

void Texture2D::unbind() { glBindTexture(target, 0); }

} // namespace Fge::Gl

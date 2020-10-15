#include "renderbuffer.hpp"
#include "gl.hpp"
#include "graphic/renderbuffer.hpp"
#include "log/log.hpp"
#include "util/assert.hpp"

namespace Fge::Gl
{

GLenum renderbuffer_format_to_gl_format(RenderBufferFormat format)
{
  switch (format)
  {
  case RenderBufferFormat::DepthComponent32:
    return GL_DEPTH_COMPONENT32;

  default:
    FGE_FAIL("No such format");
  }
}

Renderbuffer::Renderbuffer(const RenderbufferConfig &config) : config(config)
{
  glGenRenderbuffers(1, &id);
  trace("Renderbuffer", "Created renderbuffer with id: {}", id);

  glBindRenderbuffer(GL_RENDERBUFFER, id);

  if (config.samples > 0)
  {
    glRenderbufferStorageMultisample(
        GL_RENDERBUFFER,
        config.samples,
        renderbuffer_format_to_gl_format(config.format),
        config.width,
        config.height);
  }
  else
  {
    glRenderbufferStorage(GL_RENDERBUFFER,
                          renderbuffer_format_to_gl_format(config.format),
                          config.width,
                          config.height);
  }

  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

Renderbuffer::~Renderbuffer()
{
  trace("Renderbuffer", "Delete renderbuffer with id: {}", id);
  glDeleteRenderbuffers(1, &id);
}

void Renderbuffer::bind() const { glBindRenderbuffer(GL_RENDERBUFFER, id); }

void Renderbuffer::unbind() const { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

uint32_t Renderbuffer::get_id() { return id; }

} // namespace Fge::Gl

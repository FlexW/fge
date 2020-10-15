#pragma once

#include "gl.hpp"
#include "graphic/framebuffer.hpp"
#include "graphic/renderbuffer.hpp"
#include "graphic/texture.hpp"
#include "log/log.hpp"
#include "util/assert.hpp"
#include <bits/stdint-uintn.h>

namespace Fge::Gl
{

GLenum bind_mode_to_gl_bind_mode(FramebufferBindMode bind_mode)
{
  switch (bind_mode)
  {
  case FramebufferBindMode::Default:
    return GL_FRAMEBUFFER;

  case FramebufferBindMode::Draw:
    return GL_DRAW_FRAMEBUFFER;

  case FramebufferBindMode::Read:
    return GL_READ_FRAMEBUFFER;

  default:
    FGE_FAIL("No such bind mode");
  }
}

template <typename TDepth, typename TStencil, typename TColor>
class Framebuffer : public Fge::Framebuffer
{
public:
  Framebuffer(const FramebufferConfigRRT &config)
  {
    glGenFramebuffers(1, &id);
    trace("Framebuffer", "Create framebuffer with id: {}", id);
    bind();

    FGE_ASSERT(config.color_attachment);

    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           config.color_attachment->get_target(),
                           config.color_attachment->get_id(),
                           0);

    if (config.depth_attachment)
    {
      glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                GL_DEPTH_ATTACHMENT,
                                GL_RENDERBUFFER,
                                config.depth_attachment->get_id());
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      FGE_FAIL("Framebuffer not complete");
    }

    unbind();
  }

  ~Framebuffer()
  {
    trace("Framebuffer", "Delete framebuffer id: {}", id);
    glDeleteFramebuffers(1, &id);
  }

  void
  bind(FramebufferBindMode bind_mode = FramebufferBindMode::Default) override
  {
    glBindFramebuffer(bind_mode_to_gl_bind_mode(bind_mode), id);
  }

  void
  unbind(FramebufferBindMode bind_mode = FramebufferBindMode::Default) override
  {
    glBindFramebuffer(bind_mode_to_gl_bind_mode(bind_mode), 0);
  }

  uint32_t get_id() override { return id; }

private:
  uint32_t id{};

  FramebufferConfig<TDepth, TStencil, TColor> config{};
};

using FramebufferRRT =
    Framebuffer<Fge::Renderbuffer, Fge::Renderbuffer, Fge::Texture2D>;

} // namespace Fge::Gl

#pragma once

#include "graphic/renderbuffer.hpp"
#include "graphic/texture.hpp"
#include "std.hpp"

namespace Fge
{

enum class FramebufferBindMode
{
  Default,
  Draw,
  Read
};

template <typename TDepth, typename TStencil, typename TColor>
struct FramebufferConfig
{
  std::shared_ptr<TDepth>   depth_attachment{};
  std::shared_ptr<TStencil> stencil_attachment{};
  std::shared_ptr<TColor>   color_attachment{};
};

using FramebufferConfigRRT =
    FramebufferConfig<Renderbuffer, Renderbuffer, Texture2D>;

class Framebuffer
{
public:
  Framebuffer() = default;

  virtual ~Framebuffer() = default;

  virtual void
  bind(FramebufferBindMode bind_mode = FramebufferBindMode::Default) = 0;

  virtual void
  unbind(FramebufferBindMode bind_mode = FramebufferBindMode::Default) = 0;

  virtual uint32_t get_id() = 0;

private:
  Framebuffer(const Framebuffer &) = delete;

  void operator=(const Framebuffer &) = delete;
};

} // namespace Fge

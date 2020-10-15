#pragma once

#include "std.hpp"

namespace Fge
{

enum class RenderBufferFormat
{
  DepthComponent32
};

struct RenderbufferConfig
{
  RenderBufferFormat format = RenderBufferFormat::DepthComponent32;
  uint32_t           width{};
  uint32_t           height{};
  uint32_t           samples{};
};

class Renderbuffer
{
public:
  Renderbuffer() = default;

  virtual ~Renderbuffer() = default;

  virtual void bind() const = 0;

  virtual void unbind() const = 0;

  virtual uint32_t get_id() = 0;

private:
  Renderbuffer(const Renderbuffer &) = delete;

  void operator=(const Renderbuffer &) = delete;
};

} // namespace Fge

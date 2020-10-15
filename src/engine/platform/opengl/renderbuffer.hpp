#pragma once

#include "graphic/renderbuffer.hpp"

namespace Fge::Gl
{

class Renderbuffer : public Fge::Renderbuffer
{
public:
  Renderbuffer(const RenderbufferConfig &config);

  ~Renderbuffer();

  void bind() const override;

  void unbind() const override;

  uint32_t get_id() override;

private:
  uint32_t id{};

  RenderbufferConfig config;
};

} // namespace Fge::Gl

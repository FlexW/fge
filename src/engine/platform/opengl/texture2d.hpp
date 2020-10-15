#pragma once

#include "gl.hpp"
#include "graphic/texture.hpp"

namespace Fge::Gl
{

class Texture2D : public Fge::Texture2D
{
public:
  Texture2D(const Texture2DConfig &config);

  ~Texture2D();

  void bind(uint32_t slot = 0) override;

  void unbind() override;

  uint32_t get_id() override { return id; }

  uint32_t get_target() override { return target; }

private:
  GLuint id = 0;

  GLenum target = GL_TEXTURE_2D;
};

} // namespace Fge::Gl

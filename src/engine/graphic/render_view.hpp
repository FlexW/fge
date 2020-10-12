#pragma once

#include "camera.hpp"
#include "math/math.hpp"
#include "std.hpp"

class GraphicSystem;

namespace Fge
{

class RenderView
{
public:
  enum class Target
  {
    Backbuffer,
    Texture
  };

  RenderView(Target target, uint32_t width, uint32_t height, uint32_t samples);

  void render(const glm::mat4 &projection_mat,
              const Camera &   camera,
              uint32_t         width,
              uint32_t         height,
              uint32_t         samples);

  uint32_t get_target_id() const { return target_id; }

private:
  uint32_t target_id{};
};

} // namespace Fge

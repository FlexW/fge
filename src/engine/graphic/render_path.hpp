#pragma once

#include "camera.hpp"
#include "math/math.hpp"

namespace Fge
{

class RenderPath
{
public:
  virtual ~RenderPath() = default;

  virtual void render(const glm::mat4 &projection_mat,
                      const Camera &   camera,
                      uint32_t         width,
                      uint32_t         height) = 0;
};

} // namespace Fge

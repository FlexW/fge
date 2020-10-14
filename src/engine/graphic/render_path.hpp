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
                      const Camera &   camera) = 0;
};

} // namespace Fge

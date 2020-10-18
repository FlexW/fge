#pragma once

#include "camera.hpp"
#include "camera_controller.hpp"
#include "math/math.hpp"

namespace Fge
{

class RenderPath
{
public:
  virtual ~RenderPath() = default;

  virtual void render(const glm::mat4 & projection_mat,
                      const CameraInfo &camera_info,
                      uint32_t          width,
                      uint32_t          height) = 0;
};

} // namespace Fge

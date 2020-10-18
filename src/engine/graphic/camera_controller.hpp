#pragma once

#include "math/math.hpp"
#include "std.hpp"

namespace Fge
{

struct CameraInfo
{
  glm::vec3 position{};
  glm::mat4 view_mat{};
  float     zoom = 45.0f;
};

class CameraController
{
public:
  void set_camera_info(const CameraInfo &camera_info)
  {
    this->camera_info = camera_info;
  }

  const CameraInfo &get_camera_info() { return camera_info; }

private:
  CameraInfo camera_info{};
};

} // namespace Fge

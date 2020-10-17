#pragma once

#include "math/math.hpp"

namespace Fge
{

enum class CameraMovement
{
  Forward,
  Backward,
  Left,
  Right
};

const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 50.5f;
const float SENSITIVITY = 0.2f;
const float ZOOM        = 45.0f;

class Camera
{
public:
  Camera(const glm::vec3 &position = ZERO_VEC3,
         const glm::vec3 &world_up = UP_VEC,
         const float      yaw      = YAW,
         const float      pitch    = PITCH);

  glm::mat4 get_view_matrix() const;

  void process_movement(const CameraMovement direction, const float delta_time);

  void process_rotation(float      xoffset,
                        float      yoffset,
                        const bool constrain_pitch = true);

  void process_scroll(float yoffset);

  float get_zoom() const { return zoom; }

  const glm::vec3 &get_position() const { return position; }

private:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 world_up;

  float yaw;
  float pitch;

  float movement_speed;
  float mouse_sensitivity;
  float zoom;

  void update_camera_vectors();
};

} // namespace Fge

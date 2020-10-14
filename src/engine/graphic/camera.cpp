#include "camera.hpp"

namespace Fge
{

Camera::Camera(const glm::vec3 &position,
               const glm::vec3 &world_up,
               const float      yaw,
               const float      pitch)
    : position(position),
      front(glm::vec3(0.0f, 0.0f, -1.0f)),
      world_up(world_up),
      yaw(yaw),
      pitch(pitch),
      movement_speed(SPEED),
      mouse_sensitivity(SENSITIVITY),
      zoom(ZOOM)
{
  update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix() const
{
  return glm::lookAt(position, position + front, up);
}

void Camera::process_movement(const CameraMovement direction,
                              const float          delta_time)
{
  const float velocity = movement_speed * delta_time;

  if (direction == CameraMovement::Forward)
  {
    position += front * velocity;
  }
  if (direction == CameraMovement::Backward)
  {
    position -= front * velocity;
  }
  if (direction == CameraMovement::Left)
  {
    position -= right * velocity;
  }
  if (direction == CameraMovement::Right)
  {
    position += right * velocity;
  }
}

void Camera::process_rotation(float      xoffset,
                              float      yoffset,
                              const bool constrain_pitch)
{
  xoffset *= mouse_sensitivity;
  yoffset *= mouse_sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrain_pitch)
  {
    if (pitch > 89.0f)
    {
      pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
      pitch = -89.0f;
    }
  }

  // update Front, Right and Up Vectors using the updated Euler angles
  update_camera_vectors();
}

void Camera::process_scroll(float yoffset)
{
  zoom -= yoffset;

  if (zoom < 1.0f)
  {
    zoom = 1.0f;
  }
  if (zoom > 45.0f)
  {
    zoom = 45.0f;
  }
}

void Camera::update_camera_vectors()
{
  // Calculate the new Front vector
  glm::vec3 f;
  f.x   = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  f.y   = sin(glm::radians(pitch));
  f.z   = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(f);

  // Also re-calculate the Right and Up vector
  // normalize the vectors, because their length
  // gets closer to 0 the more you look up or down
  // which results in slower movement.
  right = glm::normalize(glm::cross(front, world_up));

  up = glm::normalize(glm::cross(right, front));
}

} // namespace Fge

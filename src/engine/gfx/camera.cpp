#include "camera.hpp"

namespace fge::gfx
{

camera::camera() { update_camera_vectors(); }

camera::camera(const glm::vec3 &position,
               const glm::vec3 &up,
               const float      yaw,
               const float      pitch)
    : position(position),
      up(up),
      yaw(yaw),
      pitch(pitch)
{
  update_camera_vectors();
}

glm::mat4 camera::get_view_matrix() const
{
  return glm::lookAt(position, position + front, up);
}

void camera::move(camera_movement direction, float delta_time)
{
  const auto velocity = movement_speed * delta_time;

  if (direction == camera_movement::forward)
  {
    position += front * velocity;
  }
  if (direction == camera_movement::backward)
  {
    position -= front * velocity;
  }
  if (direction == camera_movement::left)
  {
    position -= right * velocity;
  }
  if (direction == camera_movement::right)
  {
    position += right * velocity;
  }
}

void camera::rotate(float xoffset, float yoffset, bool constrain_pitch)
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

  // Update front, right and up vectors using the updated euler angles
  update_camera_vectors();
}

void camera::scroll(float yoffset)
{
  zoom -= static_cast<float>(yoffset);

  if (zoom < 1.0f)
  {
    zoom = 1.0f;
  }
  if (zoom > 45.0f)
  {
    zoom = 45.0f;
  }
}

void camera::update_camera_vectors()
{
  // Calculate the new front vector
  glm::vec3 front;
  front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
  front.y = glm::sin(glm::radians(pitch));
  front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

  this->front = glm::normalize(front);

  // Also re-calculate the right and up vector
  // Normalize the vectors, because their length
  // gets closer to 0 the more you look up or down
  // which results in slower movement.
  right = glm::normalize(glm::cross(front, world_up));
  up    = glm::normalize(glm::cross(right, front));
}

float camera::get_zoom() const { return zoom; }

} // namespace fge::gfx

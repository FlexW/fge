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
const float SPEED       = 10.5f;
const float SENSITIVITY = 0.2f;
const float ZOOM        = 45.0f;

class Camera
{
public:
  Camera(const glm::vec3 &position = ZERO_VEC3,
         const glm::vec3 &world_up = UP_VEC,
         const float      yaw      = YAW,
         const float      pitch    = PITCH)
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

  glm::mat4 get_view_matrix()
  {
    return glm::lookAt(position, position + front, up);
  }

  void process_movement(const CameraMovement direction, const float delta_time)
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

  void process_rotation(float      xoffset,
                        float      yoffset,
                        const bool constrain_pitch = true)
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

  void process_scroll(float yoffset)
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

  void update_camera_vectors()
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
};

} // namespace Fge

#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace fge::gfx
{

enum class camera_movement
{
  forward,
  backward,
  left,
  right
};

class camera
{
public:
  camera();

  camera(const glm::vec3 &position,
         const glm::vec3 &up,
         const float      yaw,
         const float      pitch);
  /**
   * @brief Calculates the view matrix calculated using Euler Angles.
   *
   * @return View matrix
   */
  glm::mat4 get_view_matrix() const;

  void move(camera_movement direction, float delta_time);

  void rotate(float xoffset, float yoffset, bool constrain_pitch = true);

  void scroll(float yoffset);

  float get_zoom() const;

private:
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 front    = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 right;

  float yaw   = -90.0f;
  float pitch = 0.0f;

  float movement_speed    = 2.5f;
  float mouse_sensitivity = 0.1f;
  float zoom              = 45.0f;

  void update_camera_vectors();
};

} // namespace fge::gfx

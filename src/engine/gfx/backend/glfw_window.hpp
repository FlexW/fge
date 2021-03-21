#pragma once

// clang-format off
#include "broadcast.hpp"
#include "opengl.hpp"
#include <GLFW/glfw3.h>
#include <bits/stdint-uintn.h>
// clang-format on

#include "gfx/graphic_backend_context.hpp"
#include "gfx/window.hpp"

namespace fge::gfx::glfw
{

class glfw_window : public window
{
public:
  glfw_window(broadcast *               application_broadcast,
              const window_create_info &create_info);

  void flush() override;

  void on_window_framebuffer_size(int width, int height);

  void on_mouse_movement(double x, double y);

  void on_scroll(double xoffset, double yoffset);

  void on_window_close();

  void on_key(int key, int scancode, int action, int mods);

  key_action get_key(key k) const override;

  void terminate() override;

  uint32_t get_window_width() const override;

  uint32_t get_window_height() const override;

  void set_capture_mouse(bool value) override;

private:
  broadcast *application_broadcast;

  GLFWwindow *window;

  bool  mouse_first_move = true;
  float mouse_last_x     = 0.0f;
  float mouse_last_y     = 0.0f;
  float mouse_offset_x = 0.0f;
  float mouse_offset_y = 0.0f;

  uint32_t window_width  = 0;
  uint32_t window_height = 0;
};

} // namespace fge::gfx::glfw

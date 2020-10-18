#pragma once

#include "glfw.hpp"

#include "graphic/window.hpp"
#include "platform/opengl/gl_window.hpp"
#include "std.hpp"

namespace Fge::Glfw
{

class GlfwWindow : public Window, public Gl::GlWindow
{
public:
  GlfwWindow();

  void init_open_gl() override;

  void create_window() override;

  void flush() override;

  void set_capture_mouse(bool value) override;

  void terminate() override;

  void on_key(int key, int scancode, int action, int mods);

  void on_window_framebuffer_size(int width, int height);

  void on_mouse_movement(double xpos, double ypos);

  void on_close();

  GLFWwindow *get_glfw_window() { return window; }

  KeyAction get_key(Key key) override;

  double get_mouse_offset_x() const override { return mouse_offset_x; }

  double get_mouse_offset_y() const override { return mouse_offset_y; }

private:
  GLFWwindow *window{};

  bool   mouse_first_move = true;
  double mouse_last_x{};
  double mouse_last_y{};
  double mouse_offset_x{};
  double mouse_offset_y{};

  bool opengl_debug = true;
};

} // namespace Fge::Glfw

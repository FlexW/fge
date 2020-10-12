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

  virtual void init_open_gl() override;

  virtual void create_window() override;

  virtual void flush() override;

  virtual void set_capture_mouse(bool value) override;

  virtual void terminate() override;

  void on_key(int key, int scancode, int action, int mods);

  void on_window_framebuffer_size(int width, int height);

  void on_mouse_movement(double xpos, double ypos);

  void on_close();

  GLFWwindow *get_glfw_window() { return window; }

private:
  GLFWwindow *window{};

  bool   mouse_first_move = true;
  double mouse_last_x{};
  double mouse_last_y{};

  bool opengl_debug = true;
};

} // namespace Fge::Glfw

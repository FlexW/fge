#include "glfw_window.hpp"
#include "GLFW/glfw3.h"
#include "application.hpp"
#include "graphic/window.hpp"
#include "log/log.hpp"
#include "platform/opengl/gl.hpp"
#include "platform/opengl/util.hpp"

#define OPENGL_VERSION_MAJOR 4
#define OPENGL_VERSION_MINOR 6

namespace Fge::Glfw
{

void glfw_error_callback(int error_code, const char *description)
{
  warning("GlfwWindow", "Glfw error {}: {}", error_code, description);
}

void window_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  auto w = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(window));

  assert(w);

  w->on_window_framebuffer_size(width, height);
}

void mouse_movement_callback(GLFWwindow *window, double xpos, double ypos)
{
  auto w = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(window));

  assert(w);

  w->on_mouse_movement(xpos, ypos);
}

void window_close_callback(GLFWwindow *window)
{
  auto w = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(window));

  assert(w);

  w->on_close();
}

void key_callback(GLFWwindow *window,
                  int         key,
                  int         scancode,
                  int         action,
                  int         mods)
{
  auto w = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(window));

  assert(w);

  w->on_key(key, scancode, action, mods);
}

GlfwWindow::GlfwWindow() {}

void GlfwWindow::init_open_gl()
{
  if (!gladLoadGL())
  {
    throw std::runtime_error("Could not initialze glad");
  }

  if (opengl_debug)
  {
    glDebugMessageCallback(Gl::debug_callback, nullptr);
    glDebugMessageControl(GL_DONT_CARE,
                          GL_DONT_CARE,
                          GL_DONT_CARE,
                          0,
                          nullptr,
                          GL_TRUE);
    glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION,
                         GL_DEBUG_TYPE_MARKER,
                         0,
                         GL_DEBUG_SEVERITY_NOTIFICATION,
                         -1,
                         "Start debugging");
  }

  Gl::dump_info();
}

void GlfwWindow::create_window()
{
  if (!glfwInit())
  {
    throw std::runtime_error("Could not initialize glfw");
  }

  glfwSetErrorCallback(glfw_error_callback);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  auto  app    = Application::get_instance();
  auto &config = app->get_config_manager()->get_config();

  opengl_debug = config["opengl"]["debug"].get<bool>();
  if (opengl_debug)
  {
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  }

  auto samples = config["window"]["samples"].get<int>();
  if (samples > 0)
  {
    glfwWindowHint(GLFW_SAMPLES, samples);
  }

  width             = config["window"]["width"].get<int>();
  height            = config["window"]["height"].get<int>();
  auto window_title = config["window"]["title"].get<const char *>();
  auto fullscreen   = config["window"]["fullscreen"].get<bool>();

  GLFWmonitor *monitor{};
  if (fullscreen)
  {
    monitor = glfwGetPrimaryMonitor();
  }

  window = glfwCreateWindow(width, height, window_title, monitor, nullptr);
  if (!window)
  {
    throw std::runtime_error("Unable to create glfw window");
  }
  glfwSetWindowUserPointer(window, this);

  glfwMakeContextCurrent(window);

  auto vsync = config["window"]["vsync"].get<bool>();
  if (!vsync)
  {
    glfwSwapInterval(0);
  }

  glfwSetFramebufferSizeCallback(window, window_framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_movement_callback);
  glfwSetWindowCloseCallback(window, window_close_callback);
  glfwSetKeyCallback(window, key_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void GlfwWindow::flush()
{
  glfwSwapBuffers(window);
  glfwPollEvents();
}

Key glfw_key_to_key(int key)
{
  switch (key)
  {
  case GLFW_KEY_W:
    return Key::W;

  case GLFW_KEY_S:
    return Key::S;

  case GLFW_KEY_A:
    return Key::A;

  case GLFW_KEY_D:
    return Key::D;

  case GLFW_KEY_ESCAPE:
    return Key::Esc;

  case GLFW_KEY_LEFT_CONTROL:
    return Key::CtrlLeft;

  default:
    FGE_FAIL("Can not handle key");
  };

  return Key::Unknown;
}

KeyAction glfw_key_action_to_key_action(int action)
{
  switch (action)
  {
  case GLFW_PRESS:
    return KeyAction::Press;

  case GLFW_RELEASE:
    return KeyAction::Release;

  case GLFW_REPEAT:
    return KeyAction::Repeat;

  default:
    FGE_FAIL("Can not handle key action");
  }

  return KeyAction::Unknown;
}

int key_to_glfw_key(Key key)
{
  switch (key)
  {
  case Key::W:
    return GLFW_KEY_W;

  case Key::S:
    return GLFW_KEY_S;

  case Key::A:
    return GLFW_KEY_A;

  case Key::D:
    return GLFW_KEY_D;

  case Key::Esc:
    return GLFW_KEY_ESCAPE;

  case Key::CtrlLeft:
    return GLFW_KEY_LEFT_CONTROL;

  default:
    FGE_FAIL("Can not handle key");
  };

  return 0;
}

void GlfwWindow::on_key(int key, int /*scancode*/, int action, int /*mods*/)
{
  auto app           = Application::get_instance();
  auto event_manager = app->get_event_manager();

  KeyEvent event{};

  event.key    = glfw_key_to_key(key);
  event.action = glfw_key_action_to_key_action(action);

  event_manager->publish(&event);
}

void GlfwWindow::on_window_framebuffer_size(int width, int height)
{
  auto app           = Application::get_instance();
  auto event_manager = app->get_event_manager();

  this->width  = width;
  this->height = height;

  WindowResizeEvent event;
  event.width  = width;
  event.height = height;

  event_manager->publish(&event);
}

void GlfwWindow::on_mouse_movement(double x, double y)
{
  auto app           = Application::get_instance();
  auto event_manager = app->get_event_manager();

  double x_offset = 0.0;
  double y_offset = 0.0;

  if (mouse_first_move)
  {
    mouse_last_x = x;
    mouse_last_y = y;

    mouse_first_move = false;
  }
  else
  {
    x_offset = x - mouse_last_x;
    y_offset = mouse_last_y - y;

    mouse_last_x = x;
    mouse_last_y = y;
  }

  MouseEvent event;
  event.x        = x;
  event.y        = y;
  event.x_offset = x_offset;
  event.y_offset = y_offset;

  event_manager->publish(&event);
}

void GlfwWindow::on_close()
{
  auto app           = Application::get_instance();
  auto event_manager = app->get_event_manager();

  WindowCloseEvent event;

  event_manager->publish(&event);
}

void GlfwWindow::set_capture_mouse(bool value)
{
  if (value)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return;
  }

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void GlfwWindow::terminate() { glfwTerminate(); }

KeyAction GlfwWindow::get_key(Key key)
{
  auto glfw_key = key_to_glfw_key(key);

  auto action = glfwGetKey(window, glfw_key);

  auto key_action = glfw_key_action_to_key_action(action);

  return key_action;
}

} // namespace Fge::Glfw

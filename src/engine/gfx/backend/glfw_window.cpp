#include <iostream>
#include <stdexcept>

// clang-format off
#include "gfx/window.hpp"
#include "opengl.hpp"
#include <GLFW/glfw3.h>
// clang-format on

#include "glfw_window.hpp"
#include "util/assert.hpp"
#include "util/log.hpp"

namespace fge::gfx::glfw
{

static void glfw_error_callback(int error_code, const char *description)
{
  error("glfw_window", "Glfw error {}: {}", error_code, description);
}

static void gl_dump_info()
{
  const GLubyte *renderer     = glGetString(GL_RENDERER);
  const GLubyte *vendor       = glGetString(GL_VENDOR);
  const GLubyte *version      = glGetString(GL_VERSION);
  const GLubyte *glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);

  GLint major, minor, samples, sampleBuffers;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);
  glGetIntegerv(GL_SAMPLES, &samples);
  glGetIntegerv(GL_SAMPLE_BUFFERS, &sampleBuffers);

  std::printf(
      "-------------------------------------------------------------\n");
  std::printf("GL Vendor    : %s\n", vendor);
  std::printf("GL Renderer  : %s\n", renderer);
  std::printf("GL Version   : %s\n", version);
  std::printf("GL Version   : %d.%d\n", major, minor);
  std::printf("GLSL Version : %s\n", glsl_version);
  std::printf("MSAA samples : %d\n", samples);
  std::printf("MSAA buffers : %d\n", sampleBuffers);
  std::printf(
      "-------------------------------------------------------------\n");
}

static void APIENTRY gl_debug_callback(GLenum source,
                                       GLenum type,
                                       GLuint id,
                                       GLenum severity,
                                       GLsizei /*length*/,
                                       const GLchar *msg,
                                       const void * /*param*/)
{
  std::string sourceStr;
  switch (source)
  {
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    sourceStr = "WindowSys";
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    sourceStr = "App";
    break;
  case GL_DEBUG_SOURCE_API:
    sourceStr = "OpenGL";
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    sourceStr = "ShaderCompiler";
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    sourceStr = "3rdParty";
    break;
  case GL_DEBUG_SOURCE_OTHER:
    sourceStr = "Other";
    break;
  default:
    sourceStr = "Unknown";
  }

  std::string typeStr;
  switch (type)
  {
  case GL_DEBUG_TYPE_ERROR:
    typeStr = "Error";
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    typeStr = "Deprecated";
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    typeStr = "Undefined";
    break;
  case GL_DEBUG_TYPE_PORTABILITY:
    typeStr = "Portability";
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    typeStr = "Performance";
    break;
  case GL_DEBUG_TYPE_MARKER:
    typeStr = "Marker";
    break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    typeStr = "PushGrp";
    break;
  case GL_DEBUG_TYPE_POP_GROUP:
    typeStr = "PopGrp";
    break;
  case GL_DEBUG_TYPE_OTHER:
    typeStr = "Other";
    break;
  default:
    typeStr = "Unknown";
  }

  std::string sevStr;
  switch (severity)
  {
  case GL_DEBUG_SEVERITY_HIGH:
    sevStr = "HIGH";
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    sevStr = "MED";
    break;
  case GL_DEBUG_SEVERITY_LOW:
    sevStr = "LOW";
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    sevStr = "NOTIFY";
    break;
  default:
    sevStr = "UNK";
  }

  std::cerr << sourceStr << ":" << typeStr << "[" << sevStr << "]"
            << "(" << id << "): " << msg << std::endl;
}

static void
window_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  auto w = static_cast<glfw_window *>(glfwGetWindowUserPointer(window));
  FGE_ASSERT(w);

  w->on_window_framebuffer_size(width, height);
}

static void
mouse_movement_callback(GLFWwindow *window, double xpos, double ypos)
{
  auto w = static_cast<glfw_window *>(glfwGetWindowUserPointer(window));
  FGE_ASSERT(w);

  w->on_mouse_movement(xpos, ypos);
}
static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  auto w = static_cast<glfw_window *>(glfwGetWindowUserPointer(window));
  FGE_ASSERT(w);

  w->on_scroll(xoffset, yoffset);
}

static void window_close_callback(GLFWwindow *window)
{
  auto w = static_cast<glfw_window *>(glfwGetWindowUserPointer(window));
  FGE_ASSERT(w);

  w->on_window_close();
}

static void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  auto w = static_cast<glfw_window *>(glfwGetWindowUserPointer(window));
  FGE_ASSERT(w);

  w->on_key(key, scancode, action, mods);
}

static key glfw_key_to_key(int key)
{
  switch (key)
  {
  case GLFW_KEY_A:
    return key::a;
  case GLFW_KEY_B:
    return key::b;
  case GLFW_KEY_C:
    return key::c;
  case GLFW_KEY_D:
    return key::d;
  case GLFW_KEY_E:
    return key::e;
  case GLFW_KEY_F:
    return key::f;
  case GLFW_KEY_G:
    return key::g;
  case GLFW_KEY_H:
    return key::h;
  case GLFW_KEY_I:
    return key::i;
  case GLFW_KEY_J:
    return key::j;
  case GLFW_KEY_K:
    return key::k;
  case GLFW_KEY_L:
    return key::l;
  case GLFW_KEY_M:
    return key::m;
  case GLFW_KEY_N:
    return key::n;
  case GLFW_KEY_O:
    return key::o;
  case GLFW_KEY_P:
    return key::p;
  case GLFW_KEY_Q:
    return key::q;
  case GLFW_KEY_R:
    return key::r;
  case GLFW_KEY_S:
    return key::s;
  case GLFW_KEY_T:
    return key::t;
  case GLFW_KEY_U:
    return key::u;
  case GLFW_KEY_V:
    return key::v;
  case GLFW_KEY_W:
    return key::w;
  case GLFW_KEY_X:
    return key::x;
  case GLFW_KEY_Y:
    return key::y;
  case GLFW_KEY_Z:
    return key::z;
  case GLFW_KEY_LEFT_CONTROL:
    return key::ctrl_left;
  case GLFW_KEY_RIGHT_CONTROL:
    return key::ctrl_right;
  case GLFW_KEY_LEFT_SUPER:
    return key::super_left;
  case GLFW_KEY_RIGHT_SUPER:
    return key::super_right;
  case GLFW_KEY_RIGHT_SHIFT:
    return key::shift_right;
  case GLFW_KEY_LEFT_SHIFT:
    return key::shift_left;
  case GLFW_KEY_LEFT:
    return key::arrow_left;
  case GLFW_KEY_RIGHT:
    return key::arrow_right;
  case GLFW_KEY_UP:
    return key::arrow_up;
  case GLFW_KEY_DOWN:
    return key::arrow_down;
  case GLFW_KEY_TAB:
    return key::tab;
  case GLFW_KEY_SPACE:
    return key::space;
  case GLFW_KEY_F1:
    return key::f1;
  case GLFW_KEY_F2:
    return key::f2;
  case GLFW_KEY_F3:
    return key::f3;
  case GLFW_KEY_F4:
    return key::f4;
  case GLFW_KEY_F5:
    return key::f5;
  case GLFW_KEY_F6:
    return key::f6;
  case GLFW_KEY_F7:
    return key::f7;
  case GLFW_KEY_F8:
    return key::f8;
  case GLFW_KEY_F9:
    return key::f9;
  case GLFW_KEY_F10:
    return key::f10;
  case GLFW_KEY_F11:
    return key::f11;
  case GLFW_KEY_F12:
    return key::f12;
  case GLFW_KEY_ESCAPE:
    return key::escape;
  default:
    FGE_ASSERT(0 && "Can not handle glfw key");
  }
}

static int key_to_glfw_key(key k)
{
  switch (k)
  {
  case key::a:
    return GLFW_KEY_A;
  case key::b:
    return GLFW_KEY_B;
  case key::c:
    return GLFW_KEY_C;
  case key::d:
    return GLFW_KEY_D;
  case key::e:
    return GLFW_KEY_E;
  case key::f:
    return GLFW_KEY_F;
  case key::g:
    return GLFW_KEY_G;
  case key::h:
    return GLFW_KEY_H;
  case key::i:
    return GLFW_KEY_I;
  case key::j:
    return GLFW_KEY_J;
  case key::k:
    return GLFW_KEY_K;
  case key::l:
    return GLFW_KEY_L;
  case key::m:
    return GLFW_KEY_M;
  case key::n:
    return GLFW_KEY_N;
  case key::o:
    return GLFW_KEY_O;
  case key::p:
    return GLFW_KEY_P;
  case key::q:
    return GLFW_KEY_Q;
  case key::r:
    return GLFW_KEY_R;
  case key::s:
    return GLFW_KEY_S;
  case key::t:
    return GLFW_KEY_T;
  case key::u:
    return GLFW_KEY_U;
  case key::v:
    return GLFW_KEY_V;
  case key::w:
    return GLFW_KEY_W;
  case key::x:
    return GLFW_KEY_X;
  case key::y:
    return GLFW_KEY_Y;
  case key::z:
    return GLFW_KEY_Z;
  case key::ctrl_left:
    return GLFW_KEY_LEFT_CONTROL;
  case key::ctrl_right:
    return GLFW_KEY_RIGHT_CONTROL;
  case key::super_left:
    return GLFW_KEY_LEFT_SUPER;
  case key::super_right:
    return GLFW_KEY_RIGHT_SUPER;
  case key::shift_right:
    return GLFW_KEY_RIGHT_SHIFT;
  case key::shift_left:
    return GLFW_KEY_LEFT_SHIFT;
  case key::arrow_left:
    return GLFW_KEY_LEFT;
  case key::arrow_right:
    return GLFW_KEY_RIGHT;
  case key::arrow_up:
    return GLFW_KEY_UP;
  case key::arrow_down:
    return GLFW_KEY_DOWN;
  case key::tab:
    return GLFW_KEY_TAB;
  case key::space:
    return GLFW_KEY_SPACE;
  case key::f1:
    return GLFW_KEY_F1;
  case key::f2:
    return GLFW_KEY_F2;
  case key::f3:
    return GLFW_KEY_F3;
  case key::f4:
    return GLFW_KEY_F4;
  case key::f5:
    return GLFW_KEY_F5;
  case key::f6:
    return GLFW_KEY_F6;
  case key::f7:
    return GLFW_KEY_F7;
  case key::f8:
    return GLFW_KEY_F8;
  case key::f9:
    return GLFW_KEY_F9;
  case key::f10:
    return GLFW_KEY_F10;
  case key::f11:
    return GLFW_KEY_F11;
  case key::f12:
    return GLFW_KEY_F12;
  case key::escape:
    return GLFW_KEY_ESCAPE;
  default:
    FGE_ASSERT(0 && "Can not handle key");
  }
}

static key_action glfw_key_action_to_key_action(int action)
{
  switch (action)
  {
  case GLFW_PRESS:
    return key_action::press;
  case GLFW_RELEASE:
    return key_action::release;
  case GLFW_REPEAT:
    return key_action::repeat;
  default:
    FGE_ASSERT(0 && "Can not handle glfw action");
  }
}

glfw_window::glfw_window(broadcast *               application_broadcast,
                         const window_create_info &create_info)
    : application_broadcast(application_broadcast)
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

  if (create_info.debug)
  {
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  }

  if (create_info.samples > 0)
  {
    glfwWindowHint(GLFW_SAMPLES, create_info.samples);
  }

  GLFWmonitor *monitor{};
  if (create_info.fullscreen)
  {
    monitor = glfwGetPrimaryMonitor();
  }

  window_width  = create_info.window_width;
  window_height = create_info.window_height;

  window = glfwCreateWindow(window_width,
                            window_height,
                            create_info.window_title.c_str(),
                            monitor,
                            nullptr);
  if (!window)
  {
    throw std::runtime_error("Unable to create glfw window");
  }
  glfwSetWindowUserPointer(window, this);

  glfwMakeContextCurrent(window);

  if (!create_info.vsync)
  {
    glfwSwapInterval(0);
  }

  glfwSetFramebufferSizeCallback(window, window_framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_movement_callback);
  glfwSetWindowCloseCallback(window, window_close_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetKeyCallback(window, key_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  if (!gladLoadGL())
  {
    throw std::runtime_error("Could not initialze glad");
  }

  if (create_info.debug)
  {
    glDebugMessageCallback(gl_debug_callback, nullptr);
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

  gl_dump_info();
}

void glfw_window::flush()
{
  mouse_offset_x = 0.0f;
  mouse_offset_y = 0.0f;

  glfwSwapBuffers(window);
  glfwPollEvents();
}

void glfw_window::on_window_framebuffer_size(int width, int height)
{
  window_width  = width;
  window_height = height;

  window_resize_event event{};
  event.new_window_width  = width;
  event.new_window_height = height;

  application_broadcast->publish(event);
}

void glfw_window::on_mouse_movement(double x, double y)
{
  if (mouse_first_move)
  {
    mouse_last_x = x;
    mouse_last_y = y;

    mouse_first_move = false;
  }

  float x_offset = x - mouse_last_x;
  // Reversed since y-coordinates go from bottom to top
  float y_offset = mouse_last_y - y;

  mouse_last_x = x;
  mouse_last_y = y;

  mouse_offset_x = x_offset;
  mouse_offset_y = y_offset;

  mouse_movement_event event{};
  event.x        = x;
  event.y        = y;
  event.x_offset = x_offset;
  event.y_offset = y_offset;

  application_broadcast->publish(event);
}

void glfw_window::on_scroll(double xoffset, double yoffset)
{
  mouse_scroll_event event{};
  event.x_offset = xoffset;
  event.y_offset = yoffset;

  application_broadcast->publish(event);
}

void glfw_window::on_window_close()
{
  window_close_event event{};
  application_broadcast->publish(event);
}

void glfw_window::on_key(int key, int /*scancode*/, int action, int /*mods*/)
{
  key_event event{};

  event.k      = glfw_key_to_key(key);
  event.action = glfw_key_action_to_key_action(action);

  application_broadcast->publish(event);
}

key_action glfw_window::get_key(key k) const
{
  auto glfw_key = key_to_glfw_key(k);

  auto action = glfwGetKey(window, glfw_key);

  auto key_action = glfw_key_action_to_key_action(action);

  return key_action;
}

void glfw_window::terminate()
{
  trace("glfw_window", "Terminate");
  glfwTerminate();
}

uint32_t glfw_window::get_window_width() const { return window_width; }

uint32_t glfw_window::get_window_height() const { return window_height; }

void glfw_window::set_capture_mouse(bool value)
{
  if (value)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return;
  }

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

} // namespace fge::gfx::glfw

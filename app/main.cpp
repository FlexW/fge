#include <iostream>
#include <stdexcept>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <fmt/format.h>

constexpr auto OPENGL_VERSION_MAJOR = 4;
constexpr auto OPENGL_VERSION_MINOR = 6;
constexpr auto SAMPLES              = 0;
constexpr auto WINDOW_WIDTH         = 800;
constexpr auto WINDOW_HEIGHT        = 600;

void gl_dump_info()
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

void APIENTRY gl_debug_callback(GLenum source,
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

void glfw_error_callback(int error_code, const char *description)
{
  fmt::print("Glfw error {}: {}", error_code, description);
}

void window_framebuffer_size_callback(GLFWwindow * /*window*/,
                                      int /*width*/,
                                      int /*height*/)
{
}

void mouse_movement_callback(GLFWwindow * /*window*/,
                             double /*xpos*/,
                             double /*ypos*/)
{
}

void window_close_callback(GLFWwindow * /*window*/) {}

void key_callback(GLFWwindow * /*window*/,
                  int /*key*/,
                  int /*scancode*/,
                  int /*action*/,
                  int /*mods*/)
{
}

int main(/*int argc, char *argv[]*/)
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

  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  glfwWindowHint(GLFW_SAMPLES, SAMPLES);

  auto window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Fge", nullptr, nullptr);

  if (!window)
  {
    throw std::runtime_error("Unable to create glfw window");
  }

  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(window, window_framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_movement_callback);
  glfwSetWindowCloseCallback(window, window_close_callback);
  glfwSetKeyCallback(window, key_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  if (!gladLoadGL())
  {
    throw std::runtime_error("Could not initialze glad");
  }

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

  gl_dump_info();

  while (!glfwWindowShouldClose(window))
  {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}

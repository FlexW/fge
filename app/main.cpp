#include <iostream>
#include <stdexcept>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stb_image.h>

#include "shader.hpp"
#include "camera.hpp"
#include "commands.hpp"
#include "command_bucket.hpp"

using namespace fge;

constexpr auto OPENGL_VERSION_MAJOR = 4;
constexpr auto OPENGL_VERSION_MINOR = 6;
constexpr auto SAMPLES              = 0;

auto window_width  = 800;
auto window_height = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float  lastX      = window_width / 2.0f;
float  lastY      = window_height / 2.0f;
bool   firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
                                      int width,
                                      int height)
{
  window_width  = width;
  window_height = height;
  glViewport(0, 0, width, height);
}

void mouse_movement_callback(GLFWwindow * /*window*/, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX      = xpos;
    lastY      = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow * /*window*/,
                     double /*xoffset*/,
                     double yoffset)
{
  camera.ProcessMouseScroll(yoffset);
}

void window_close_callback(GLFWwindow * /*window*/) {}

void key_callback(GLFWwindow * /*window*/,
                  int /*key*/,
                  int /*scancode*/,
                  int /*action*/,
                  int /*mods*/)
{
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
// world space positions of our cubes
glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::vec3(2.0f, 5.0f, -15.0f),
                             glm::vec3(-1.5f, -2.2f, -2.5f),
                             glm::vec3(-3.8f, -2.0f, -12.3f),
                             glm::vec3(2.4f, -0.4f, -3.5f),
                             glm::vec3(-1.7f, 3.0f, -7.5f),
                             glm::vec3(1.3f, -2.0f, -2.5f),
                             glm::vec3(1.5f, 2.0f, -2.5f),
                             glm::vec3(1.5f, 0.2f, -1.5f),
                             glm::vec3(-1.3f, 1.0f, -1.5f)};

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
      glfwCreateWindow(window_width, window_height, "Fge", nullptr, nullptr);

  if (!window)
  {
    glfwTerminate();
    throw std::runtime_error("Unable to create glfw window");
  }

  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(window, window_framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_movement_callback);
  glfwSetWindowCloseCallback(window, window_close_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGL())
  {
    glfwTerminate();
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

  std::ifstream vertex_shader_source_ifs("../resources/shaders/shader.vert");
  FGE_ASSERT(vertex_shader_source_ifs.is_open());
  std::string vertex_shader_source(
      static_cast<std::stringstream const &>(
          std::stringstream() << vertex_shader_source_ifs.rdbuf())
          .str());
  vertex_shader_source_ifs.close();

  std::ifstream fragment_shader_source_ifs("../resources/shaders/shader.frag");
  FGE_ASSERT(fragment_shader_source_ifs.is_open());
  std::string fragment_shader_source(
      static_cast<std::stringstream const &>(
          std::stringstream() << fragment_shader_source_ifs.rdbuf())
          .str());
  fragment_shader_source_ifs.close();

  const auto shader_program =
      gfx::create_shader_program(vertex_shader_source, fragment_shader_source);

  gfx::vertex_buffer_layout vb_layout;
  vb_layout.add_float(3);
  vb_layout.add_float(2);

  gfx::memory vb_memory{};
  vb_memory.data = vertices;
  vb_memory.size = sizeof(vertices);

  const auto vertex_buffer = gfx::create_vertex_buffer(vb_memory, vb_layout);

  int width, height, nrChannels;
  // tell stb_image.h to flip loaded texture's on the y-axis.
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("../resources/textures/container.jpg",
                                  &width,
                                  &height,
                                  &nrChannels,
                                  0);
  if (!data)
  {
    std::cout << "Failed to load texture" << std::endl;
  }

  gfx::texture_info tex_info1{};
  tex_info1.type                 = gfx::texture_type::two_d;
  tex_info1.width                = width;
  tex_info1.height               = height;
  tex_info1.wrap_u               = gfx::wrap_mode::repeat;
  tex_info1.wrap_v               = gfx::wrap_mode::repeat;
  tex_info1.minification_filter  = gfx::filter_mode::linear;
  tex_info1.magnification_filter = gfx::filter_mode::linear;
  tex_info1.internal_format      = gfx::pixel_internal_format::rgb;
  tex_info1.source_format        = gfx::pixel_format::rgb;
  tex_info1.source_type          = gfx::pixel_type::ubyte;
  tex_info1.generate_mipmap      = true;

  const auto texture1 = gfx::create_texture(data, tex_info1);

  stbi_image_free(data);

  data = stbi_load("../resources/textures/awesomeface.png",
                   &width,
                   &height,
                   &nrChannels,
                   0);
  if (!data)
  {
    std::cout << "Failed to load texture" << std::endl;
  }

  gfx::texture_info tex_info2{};
  tex_info2.type                 = gfx::texture_type::two_d;
  tex_info2.width                = width;
  tex_info2.height               = height;
  tex_info2.wrap_u               = gfx::wrap_mode::repeat;
  tex_info2.wrap_v               = gfx::wrap_mode::repeat;
  tex_info2.minification_filter  = gfx::filter_mode::linear;
  tex_info2.magnification_filter = gfx::filter_mode::linear;
  tex_info2.internal_format      = gfx::pixel_internal_format::rgb;
  tex_info2.source_format        = gfx::pixel_format::rgba;
  tex_info2.source_type          = gfx::pixel_type::ubyte;
  tex_info2.generate_mipmap      = true;

  const auto texture2 = gfx::create_texture(data, tex_info2);

  stbi_image_free(data);

  gfx::init_buckets();
  const auto bucket = gfx::command_bucket::create();
  bucket->set_clear_color(0.2f, 0.3f, 0.3f, 1.0f);
  bucket->set_state(gfx::render_state::clear_color |
                    gfx::render_state::clear_depth);

  while (!glfwWindowShouldClose(window))
  {
    float currentFrame = glfwGetTime();
    deltaTime          = currentFrame - lastFrame;
    lastFrame          = currentFrame;

    processInput(window);

    gfx::start_frame();
    bucket->clear();

    glm::mat4 projection =
        glm::perspective(glm::radians(camera.Zoom),
                         (float)window_width / (float)window_height,
                         0.1f,
                         100.0f);

    glm::mat4 view = camera.GetViewMatrix();

    for (unsigned int i = 0; i < 10; ++i)
    {
      glm::mat4 model  = glm::mat4(1.0f);
      model            = glm::translate(model, cubePositions[i]);
      const auto angle = 20.0f * i;
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

      auto set_proj_mat_command =
          bucket->new_command<gfx::set_uniform_command<glm::mat4>>();
      set_proj_mat_command->shader = shader_program;
      set_proj_mat_command->type   = gfx::uniform_type::mat4;
      set_proj_mat_command->name   = "projection";
      set_proj_mat_command->data   = projection;

      auto set_view_mat_command =
          bucket->append_command<gfx::set_uniform_command<glm::mat4>>(
              set_proj_mat_command);
      set_view_mat_command->shader = shader_program;
      set_view_mat_command->type   = gfx::uniform_type::mat4;
      set_view_mat_command->name   = "view";
      set_view_mat_command->data   = view;

      auto set_model_mat_command =
          bucket->append_command<gfx::set_uniform_command<glm::mat4>>(
              set_view_mat_command);
      set_model_mat_command->shader = shader_program;
      set_model_mat_command->type   = gfx::uniform_type::mat4;
      set_model_mat_command->name   = "model";
      set_model_mat_command->data   = model;

      auto set_tex2d1_command =
          bucket->append_command<gfx::set_uniform_command<gfx::texture_handle>>(
              set_model_mat_command);
      set_tex2d1_command->shader = shader_program;
      set_tex2d1_command->type   = gfx::uniform_type::texture2d;
      set_tex2d1_command->name   = "texture1";
      set_tex2d1_command->data   = texture1;

      auto set_tex2d2_command =
          bucket->append_command<gfx::set_uniform_command<gfx::texture_handle>>(
              set_tex2d1_command);
      set_tex2d2_command->shader = shader_program;
      set_tex2d2_command->type   = gfx::uniform_type::texture2d;
      set_tex2d2_command->name   = "texture2";
      set_tex2d2_command->data   = texture2;

      auto draw_cube_command =
          bucket->append_command<gfx::draw_command>(set_tex2d2_command);
      draw_cube_command->vertices = vertex_buffer;
      draw_cube_command->shader     = shader_program;
      draw_cube_command->depth_test = true;
    }

    gfx::render_frame();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

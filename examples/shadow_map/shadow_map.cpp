#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

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

#include "camera.hpp"
#include "commands.hpp"
#include "command_bucket.hpp"
#include "root_directory.hpp"

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

  gfx::init_buckets();

  // ---------------------------------------------------------------------------
  // Example init start
  // ---------------------------------------------------------------------------

  std::ifstream vertex_shader_source_ifs(
      (logl_root + "/resources/shaders/shadow_mapping.vert").c_str());
  FGE_ASSERT(vertex_shader_source_ifs.is_open());
  std::string vertex_shader_source(
      static_cast<std::stringstream const &>(
          std::stringstream() << vertex_shader_source_ifs.rdbuf())
          .str());
  vertex_shader_source_ifs.close();

  std::ifstream fragment_shader_source_ifs(
      (logl_root + "/resources/shaders/shadow_mapping.frag").c_str());
  FGE_ASSERT(fragment_shader_source_ifs.is_open());
  std::string fragment_shader_source(
      static_cast<std::stringstream const &>(
          std::stringstream() << fragment_shader_source_ifs.rdbuf())
          .str());
  fragment_shader_source_ifs.close();

  const auto shader =
      gfx::create_shader_program(vertex_shader_source, fragment_shader_source);

  std::ifstream vertex_simple_depth_shader_source_ifs(
      (logl_root + "/resources/shaders/shadow_mapping_depth.vert").c_str());
  FGE_ASSERT(vertex_simple_depth_shader_source_ifs.is_open());
  std::string vertex_simple_depth_shader_source(
      static_cast<std::stringstream const &>(
          std::stringstream() << vertex_simple_depth_shader_source_ifs.rdbuf())
          .str());
  vertex_simple_depth_shader_source_ifs.close();

  std::ifstream fragment_simple_depth_shader_source_ifs(
      (logl_root + "/resources/shaders/shadow_mapping_depth.frag").c_str());
  FGE_ASSERT(fragment_simple_depth_shader_source_ifs.is_open());
  std::string fragment_simple_depth_shader_source(
      static_cast<std::stringstream const &>(
          std::stringstream()
          << fragment_simple_depth_shader_source_ifs.rdbuf())
          .str());
  fragment_simple_depth_shader_source_ifs.close();

  const auto simple_depth_shader =
      gfx::create_shader_program(vertex_simple_depth_shader_source,
                                 fragment_simple_depth_shader_source);

  float plane_vertices[] = {
      // positions            // normals         // texcoords
      25.0f,  -0.5f, 25.0f,  0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
      -25.0f, -0.5f, 25.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f,
      -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f,  25.0f,

      25.0f,  -0.5f, 25.0f,  0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
      -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f,  25.0f,
      25.0f,  -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f};

  float cube_vertices[] = {
      // back face
      -1.0f,
      -1.0f,
      -1.0f,
      0.0f,
      0.0f,
      -1.0f,
      0.0f,
      0.0f, // bottom-left
      1.0f,
      1.0f,
      -1.0f,
      0.0f,
      0.0f,
      -1.0f,
      1.0f,
      1.0f, // top-right
      1.0f,
      -1.0f,
      -1.0f,
      0.0f,
      0.0f,
      -1.0f,
      1.0f,
      0.0f, // bottom-right
      1.0f,
      1.0f,
      -1.0f,
      0.0f,
      0.0f,
      -1.0f,
      1.0f,
      1.0f, // top-right
      -1.0f,
      -1.0f,
      -1.0f,
      0.0f,
      0.0f,
      -1.0f,
      0.0f,
      0.0f, // bottom-left
      -1.0f,
      1.0f,
      -1.0f,
      0.0f,
      0.0f,
      -1.0f,
      0.0f,
      1.0f, // top-left
      // front face
      -1.0f,
      -1.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f,
      0.0f,
      0.0f, // bottom-left
      1.0f,
      -1.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f,
      1.0f,
      0.0f, // bottom-right
      1.0f,
      1.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f,
      1.0f,
      1.0f, // top-right
      1.0f,
      1.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f,
      1.0f,
      1.0f, // top-right
      -1.0f,
      1.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f,
      0.0f,
      1.0f, // top-left
      -1.0f,
      -1.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f,
      0.0f,
      0.0f, // bottom-left
      // left face
      -1.0f,
      1.0f,
      1.0f,
      -1.0f,
      0.0f,
      0.0f,
      1.0f,
      0.0f, // top-right
      -1.0f,
      1.0f,
      -1.0f,
      -1.0f,
      0.0f,
      0.0f,
      1.0f,
      1.0f, // top-left
      -1.0f,
      -1.0f,
      -1.0f,
      -1.0f,
      0.0f,
      0.0f,
      0.0f,
      1.0f, // bottom-left
      -1.0f,
      -1.0f,
      -1.0f,
      -1.0f,
      0.0f,
      0.0f,
      0.0f,
      1.0f, // bottom-left
      -1.0f,
      -1.0f,
      1.0f,
      -1.0f,
      0.0f,
      0.0f,
      0.0f,
      0.0f, // bottom-right
      -1.0f,
      1.0f,
      1.0f,
      -1.0f,
      0.0f,
      0.0f,
      1.0f,
      0.0f, // top-right
            // right face
      1.0f,
      1.0f,
      1.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f,
      0.0f, // top-left
      1.0f,
      -1.0f,
      -1.0f,
      1.0f,
      0.0f,
      0.0f,
      0.0f,
      1.0f, // bottom-right
      1.0f,
      1.0f,
      -1.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f,
      1.0f, // top-right
      1.0f,
      -1.0f,
      -1.0f,
      1.0f,
      0.0f,
      0.0f,
      0.0f,
      1.0f, // bottom-right
      1.0f,
      1.0f,
      1.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f,
      0.0f, // top-left
      1.0f,
      -1.0f,
      1.0f,
      1.0f,
      0.0f,
      0.0f,
      0.0f,
      0.0f, // bottom-left
      // bottom face
      -1.0f,
      -1.0f,
      -1.0f,
      0.0f,
      -1.0f,
      0.0f,
      0.0f,
      1.0f, // top-right
      1.0f,
      -1.0f,
      -1.0f,
      0.0f,
      -1.0f,
      0.0f,
      1.0f,
      1.0f, // top-left
      1.0f,
      -1.0f,
      1.0f,
      0.0f,
      -1.0f,
      0.0f,
      1.0f,
      0.0f, // bottom-left
      1.0f,
      -1.0f,
      1.0f,
      0.0f,
      -1.0f,
      0.0f,
      1.0f,
      0.0f, // bottom-left
      -1.0f,
      -1.0f,
      1.0f,
      0.0f,
      -1.0f,
      0.0f,
      0.0f,
      0.0f, // bottom-right
      -1.0f,
      -1.0f,
      -1.0f,
      0.0f,
      -1.0f,
      0.0f,
      0.0f,
      1.0f, // top-right
      // top face
      -1.0f,
      1.0f,
      -1.0f,
      0.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f, // top-left
      1.0f,
      1.0f,
      1.0f,
      0.0f,
      1.0f,
      0.0f,
      1.0f,
      0.0f, // bottom-right
      1.0f,
      1.0f,
      -1.0f,
      0.0f,
      1.0f,
      0.0f,
      1.0f,
      1.0f, // top-right
      1.0f,
      1.0f,
      1.0f,
      0.0f,
      1.0f,
      0.0f,
      1.0f,
      0.0f, // bottom-right
      -1.0f,
      1.0f,
      -1.0f,
      0.0f,
      1.0f,
      0.0f,
      0.0f,
      1.0f, // top-left
      -1.0f,
      1.0f,
      1.0f,
      0.0f,
      1.0f,
      0.0f,
      0.0f,
      0.0f // bottom-left
  };

  gfx::vertex_buffer_layout plane_vertices_layout;
  plane_vertices_layout.add_float(3).add_float(3).add_float(2);
  const auto plane_vertex_buffer =
      gfx::create_vertex_buffer({plane_vertices, sizeof(plane_vertices)},
                                plane_vertices_layout);

  gfx::vertex_buffer_layout cube_vertices_layout;
  cube_vertices_layout.add_float(3).add_float(3).add_float(2);
  const auto cube_vertex_buffer =
      gfx::create_vertex_buffer({cube_vertices, sizeof(cube_vertices)},
                                cube_vertices_layout);

  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  const auto wood_texture_data =
      stbi_load((logl_root + "/resources/textures/wood.png").c_str(),
                &width,
                &height,
                &nrChannels,
                0);
  if (!wood_texture_data)
  {
    std::cout << "Failed to load texture" << std::endl;
  }

  gfx::texture_info wood_texture_info;
  wood_texture_info.type                 = gfx::texture_type::two_d;
  wood_texture_info.width                = width;
  wood_texture_info.height               = height;
  wood_texture_info.wrap_u               = gfx::wrap_mode::repeat;
  wood_texture_info.wrap_v               = gfx::wrap_mode::repeat;
  wood_texture_info.minification_filter =
      gfx::filter_mode::linear_mipmap_linear;
  wood_texture_info.magnification_filter = gfx::filter_mode::linear;
  wood_texture_info.internal_format      = gfx::pixel_internal_format::rgb;
  wood_texture_info.source_format        = gfx::pixel_format::rgb;
  wood_texture_info.source_type          = gfx::pixel_type::ubyte;
  wood_texture_info.generate_mipmap      = true;
  const auto wood_texture =
      gfx::create_texture(wood_texture_data, wood_texture_info);

  stbi_image_free(wood_texture_data);

  const uint32_t shadow_width = 1024, shadow_height = 1024;

  gfx::texture_info shadow_map_depth_attachment_texture_info;
  shadow_map_depth_attachment_texture_info.type = gfx::texture_type::two_d;
  shadow_map_depth_attachment_texture_info.width = shadow_width;
  shadow_map_depth_attachment_texture_info.height = shadow_height;
  shadow_map_depth_attachment_texture_info.wrap_u =
      gfx::wrap_mode::clamp_to_border;
  shadow_map_depth_attachment_texture_info.wrap_v =
      gfx::wrap_mode::clamp_to_border;
  shadow_map_depth_attachment_texture_info.minification_filter =
      gfx::filter_mode::nearest;
  shadow_map_depth_attachment_texture_info.magnification_filter =
      gfx::filter_mode::nearest;
  shadow_map_depth_attachment_texture_info.internal_format =
      gfx::pixel_internal_format::depth;
  shadow_map_depth_attachment_texture_info.source_format =
      gfx::pixel_format::depth;
  shadow_map_depth_attachment_texture_info.source_type =
      gfx::pixel_type::floating;
  shadow_map_depth_attachment_texture_info.generate_mipmap     = true;
  shadow_map_depth_attachment_texture_info.enable_border_color = true;
  shadow_map_depth_attachment_texture_info.border_color[0]     = 1.0;
  shadow_map_depth_attachment_texture_info.border_color[1]     = 1.0;
  shadow_map_depth_attachment_texture_info.border_color[2]     = 1.0;
  shadow_map_depth_attachment_texture_info.border_color[3]     = 1.0;

  const auto shadow_map_depth_attachment_texture =
      gfx::create_texture(nullptr, shadow_map_depth_attachment_texture_info);

  gfx::attachment_info shadow_map_depth_attachment_info;
  shadow_map_depth_attachment_info.type = gfx::attachment_type::depth;
  shadow_map_depth_attachment_info.target_type = gfx::render_target::texture;
  shadow_map_depth_attachment_info.handle = shadow_map_depth_attachment_texture;

  const auto shadow_map_framebuffer =
      gfx::create_framebuffer({shadow_map_depth_attachment_info});

  glm::vec3 light_pos(-2.0f, 4.0f, -1.0f);

  const auto shadow_map_bucket = gfx::command_bucket::create();
  shadow_map_bucket->set_clear_color(0.0f, 0.0f, 0.0f, 1.0f);
  shadow_map_bucket->set_state(gfx::render_state::clear_depth);
  shadow_map_bucket->set_framebuffer(shadow_map_framebuffer);

  const auto bucket = gfx::command_bucket::create();
  bucket->set_clear_color(0.1f, 0.1f, 0.1f, 1.0f);
  bucket->set_state(gfx::render_state::clear_depth |
                    gfx::render_state::clear_color);

  // ---------------------------------------------------------------------------
  // Example init end
  // ---------------------------------------------------------------------------

  while (!glfwWindowShouldClose(window))
  {
    float currentFrame = glfwGetTime();
    deltaTime          = currentFrame - lastFrame;
    lastFrame          = currentFrame;

    processInput(window);

    gfx::start_frame();

    // ---------------------------------------------------------------------------
    // Example update start
    // ---------------------------------------------------------------------------

    shadow_map_bucket->clear();
    bucket->clear();

    shadow_map_bucket->set_viewport(0, 0, shadow_width, shadow_height);

    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float     near_plane = 1.0f, far_plane = 7.5f;
    // lightProjection = glm::perspective(glm::radians(45.0f),
    // (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); //
    // note that if you use a perspective projection matrix you'll have to change
    // the light position as the current light position isn't enough to reflect
    // the whole scene
    lightProjection =
        glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView =
        glm::lookAt(light_pos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    {
      // Floor
      auto command =
          shadow_map_bucket->new_command<gfx::set_uniform_command<glm::mat4>>();
      command->shader = simple_depth_shader;
      command->type   = gfx::uniform_type::mat4;
      command->name   = "lightSpaceMatrix";
      command->data   = lightSpaceMatrix;

      auto command2 =
          shadow_map_bucket
              ->append_command<gfx::set_uniform_command<glm::mat4>>(command);
      command2->shader = simple_depth_shader;
      command2->type   = gfx::uniform_type::mat4;
      command2->name   = "model";
      command2->data   = glm::mat4(1.0f);

      auto draw_command =
          shadow_map_bucket->append_command<gfx::draw_command>(command2);
      draw_command->depth_test = true;
      draw_command->shader     = simple_depth_shader;
      draw_command->vertices   = plane_vertex_buffer;

      // Cubes
      command =
          shadow_map_bucket->new_command<gfx::set_uniform_command<glm::mat4>>();
      command->shader = simple_depth_shader;
      command->type   = gfx::uniform_type::mat4;
      command->name   = "lightSpaceMatrix";
      command->data   = lightSpaceMatrix;

      auto model = glm::mat4(1.0f);
      model      = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
      model      = glm::scale(model, glm::vec3(0.5f));

      command2 =
          shadow_map_bucket
              ->append_command<gfx::set_uniform_command<glm::mat4>>(command);
      command2->shader = simple_depth_shader;
      command2->type   = gfx::uniform_type::mat4;
      command2->name   = "model";
      command2->data   = model;

      draw_command =
          shadow_map_bucket->append_command<gfx::draw_command>(command2);
      draw_command->depth_test = true;
      draw_command->shader     = simple_depth_shader;
      draw_command->vertices   = cube_vertex_buffer;

      command =
          shadow_map_bucket->new_command<gfx::set_uniform_command<glm::mat4>>();
      command->shader = simple_depth_shader;
      command->type   = gfx::uniform_type::mat4;
      command->name   = "lightSpaceMatrix";
      command->data   = lightSpaceMatrix;

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
      model = glm::scale(model, glm::vec3(0.5f));

      command2 =
          shadow_map_bucket
              ->append_command<gfx::set_uniform_command<glm::mat4>>(command);
      command2->shader = simple_depth_shader;
      command2->type   = gfx::uniform_type::mat4;
      command2->name   = "model";
      command2->data   = model;

      draw_command =
          shadow_map_bucket->append_command<gfx::draw_command>(command2);
      draw_command->depth_test = true;
      draw_command->shader     = simple_depth_shader;
      draw_command->vertices   = cube_vertex_buffer;

      command =
          shadow_map_bucket->new_command<gfx::set_uniform_command<glm::mat4>>();
      command->shader = simple_depth_shader;
      command->type   = gfx::uniform_type::mat4;
      command->name   = "lightSpaceMatrix";
      command->data   = lightSpaceMatrix;

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
      model = glm::rotate(model,
                          glm::radians(60.0f),
                          glm::normalize(glm::vec3(1.0, 0.0, 1.0)));

      command2 =
          shadow_map_bucket
              ->append_command<gfx::set_uniform_command<glm::mat4>>(command);
      command2->shader = simple_depth_shader;
      command2->type   = gfx::uniform_type::mat4;
      command2->name   = "model";
      command2->data   = model;

      draw_command =
          shadow_map_bucket->append_command<gfx::draw_command>(command2);
      draw_command->depth_test = true;
      draw_command->shader     = simple_depth_shader;
      draw_command->vertices   = cube_vertex_buffer;
    }

    bucket->set_viewport(0, 0, window_width, window_height);

    {
      const auto prepare_draw = [&]() {
        glm::mat4 projection =
            glm::perspective(glm::radians(camera.Zoom),
                             (float)window_width / (float)window_height,
                             0.1f,
                             100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        auto command =
            bucket->new_command<gfx::set_uniform_command<glm::mat4>>();
        command->shader = shader;
        command->type   = gfx::uniform_type::mat4;
        command->name   = "projection";
        command->data   = projection;

        auto command2 =
            bucket->append_command<gfx::set_uniform_command<glm::mat4>>(
                command);
        command2->shader = shader;
        command2->type   = gfx::uniform_type::mat4;
        command2->name   = "view";
        command2->data   = view;

        auto command3 =
            bucket->append_command<gfx::set_uniform_command<glm::vec3>>(
                command2);
        command3->shader = shader;
        command3->type   = gfx::uniform_type::vec3;
        command3->name   = "viewPos";
        command3->data   = camera.Position;

        auto command4 =
            bucket->append_command<gfx::set_uniform_command<glm::vec3>>(
                command3);
        command4->shader = shader;
        command4->type   = gfx::uniform_type::vec3;
        command4->name   = "lightPos";
        command4->data   = light_pos;

        auto command5 =
            bucket->append_command<gfx::set_uniform_command<glm::mat4>>(
                command4);
        command5->shader = shader;
        command5->type   = gfx::uniform_type::mat4;
        command5->name   = "lightSpaceMatrix";
        command5->data   = lightSpaceMatrix;

        auto command6 =
            bucket
                ->append_command<gfx::set_uniform_command<gfx::texture_handle>>(
                    command5);
        command6->shader = shader;
        command6->type   = gfx::uniform_type::texture2d;
        command6->name   = "diffuseTexture";
        command6->data   = wood_texture;

        auto command7 =
            bucket
                ->append_command<gfx::set_uniform_command<gfx::texture_handle>>(
                    command6);
        command7->shader = shader;
        command7->type   = gfx::uniform_type::texture2d;
        command7->name   = "shadowMap";
        command7->data   = shadow_map_depth_attachment_texture;

        return command7;
      };

      // Floor
      auto command = prepare_draw();

      glm::mat4 model = glm::mat4(1.0f);

      auto command2 =
          bucket->append_command<gfx::set_uniform_command<glm::mat4>>(command);
      command2->shader = shader;
      command2->type   = gfx::uniform_type::mat4;
      command2->name   = "model";
      command2->data   = model;

      auto command3    = bucket->append_command<gfx::draw_command>(command2);
      command3->shader     = shader;
      command3->depth_test = true;
      command3->vertices   = plane_vertex_buffer;

      // Cubes
      command = prepare_draw();

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
      model = glm::scale(model, glm::vec3(0.5f));

      command2 =
          bucket->append_command<gfx::set_uniform_command<glm::mat4>>(command);
      command2->shader = shader;
      command2->type   = gfx::uniform_type::mat4;
      command2->name   = "model";
      command2->data   = model;

      command3         = bucket->append_command<gfx::draw_command>(command2);
      command3->shader = shader;
      command3->depth_test = true;
      command3->vertices   = cube_vertex_buffer;

      command = prepare_draw();

      model   = glm::mat4(1.0f);
      model   = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
      model   = glm::scale(model, glm::vec3(0.5f));

      command2 =
          bucket->append_command<gfx::set_uniform_command<glm::mat4>>(command);
      command2->shader = shader;
      command2->type   = gfx::uniform_type::mat4;
      command2->name   = "model";
      command2->data   = model;

      command3         = bucket->append_command<gfx::draw_command>(command2);
      command3->shader = shader;
      command3->depth_test = true;
      command3->vertices   = cube_vertex_buffer;

      command = prepare_draw();

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
      model = glm::rotate(model,
                          glm::radians(60.0f),
                          glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
      model = glm::scale(model, glm::vec3(0.25));

      command2 =
          bucket->append_command<gfx::set_uniform_command<glm::mat4>>(command);
      command2->shader = shader;
      command2->type   = gfx::uniform_type::mat4;
      command2->name   = "model";
      command2->data   = model;

      command3         = bucket->append_command<gfx::draw_command>(command2);
      command3->shader = shader;
      command3->depth_test = true;
      command3->vertices   = cube_vertex_buffer;
    }

    // ---------------------------------------------------------------------------
    // Example update end
    // ---------------------------------------------------------------------------

    gfx::render_frame();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

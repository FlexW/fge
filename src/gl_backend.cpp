#include <cassert>
#include <glad/glad.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "command_bucket.hpp"
#include "commands.hpp"
#include "gl_backend.hpp"

namespace fge::gfx::gl
{

struct vertex_buffer
{
  GLuint      id{};
  GLuint      vao{};
  std::size_t size;
};

struct texture
{
  GLuint id;
  GLint  type;
};

struct shader
{
  GLuint id;
  GLuint next_free_texture_slot = 0;
};

static std::vector<vertex_buffer> vertex_buffers;
static std::vector<texture>       textures;
static std::vector<shader>        shaders;

struct gl_type_info
{
  GLuint      type;
  std::size_t size;
};

static GLenum to_gl(wrap_mode mode)
{
  switch (mode)
  {
  case wrap_mode::clamp_to_edge:
    return GL_CLAMP_TO_EDGE;
  case wrap_mode::repeat:
    return GL_REPEAT;
  default:
    FGE_ASSERT(0 && "Can not handle wrap mode");
  }
}

static GLenum to_gl(filter_mode mode)
{
  switch (mode)
  {
  case filter_mode::linear:
    return GL_LINEAR;
  default:
    FGE_ASSERT(0 && "Can not handle filter mode");
  }
}

static GLint to_gl(pixel_internal_format format)
{
  switch (format)
  {
  case pixel_internal_format::rgb:
    return GL_RGB;
  default:
    FGE_ASSERT(0 && "Can not handle pixel internal format");
  }
}

static GLint to_gl(pixel_format format)
{
  switch (format)
  {
  case pixel_format::rgb:
    return GL_RGB;
  case pixel_format::rgba:
    return GL_RGBA;
  default:
    FGE_ASSERT(0 && "Can not handle pixel format");
  }
}

static GLint to_gl(pixel_type type)
{
  switch (type)
  {
  case pixel_type::ubyte:
    return GL_UNSIGNED_BYTE;
  default:
    FGE_ASSERT(0 && "Can not handle pixel type");
  }
}

static GLint to_gl(texture_type type)
{
  switch (type)
  {
  case texture_type::two_d:
    return GL_TEXTURE_2D;
  default:
    FGE_ASSERT(0 && "Can not handle texture type");
  }
}

static gl_type_info get_gl_type_info(attribute_type type)
{
  std::size_t real_type_size;
  GLuint      real_type;

  switch (type)
  {
  case attribute_type::floating:
    real_type      = GL_FLOAT;
    real_type_size = sizeof(GL_FLOAT);
    break;
  default:
    assert(0 && "Can not handle type");
  }

  gl_type_info info{};
  info.type = real_type;
  info.size = real_type_size;

  return info;
}

vertex_buffer_handle create_vertex_buffer(const memory &              memory,
                                          const vertex_buffer_layout &layout)
{
  GLuint vbo{};
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, memory.size, memory.data, GL_STATIC_DRAW);

  GLuint vao{};
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);

  // Calculate stride
  std::size_t stride = 0;
  for (const auto &element : layout.get_elements())
  {
    const auto gl_type = get_gl_type_info(element.type);
    stride += gl_type.size * element.count;
  }

  std::size_t offset     = 0;
  std::size_t index      = 0;
  const bool  normalized = false;

  for (const auto &element : layout.get_elements())
  {
    const auto gl_type = get_gl_type_info(element.type);

    glEnableVertexAttribArray(index);

    switch (gl_type.type)
    {
    case GL_FLOAT:
      glVertexAttribPointer(index,
                            element.count,
                            gl_type.type,
                            normalized,
                            stride,
                            reinterpret_cast<const void *>(offset));
      break;
    case GL_INT:
      glVertexAttribIPointer(index,
                             element.count,
                             GL_INT,
                             stride,
                             reinterpret_cast<const void *>(offset));
      break;

    case GL_UNSIGNED_INT:
      glVertexAttribIPointer(index,
                             element.count,
                             GL_UNSIGNED_INT,
                             stride,
                             reinterpret_cast<const void *>(offset));
      break;
    default:
      assert(0 && "Can not handle type");
    };

    offset += element.count * gl_type.size;
    ++index;
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  vertex_buffer vertex_buffer;
  vertex_buffer.id   = vbo;
  vertex_buffer.vao  = vao;
  vertex_buffer.size = memory.size;

  const vertex_buffer_handle handle = vertex_buffers.size();
  vertex_buffers.push_back(vertex_buffer);

  return handle;
}

enum class shader_type
{
  vertex,
  fragment
};

static void check_compile_errors(GLuint shader_id, shader_type type)
{
  GLint          success{};
  constexpr auto info_log_size = 1024;
  GLchar         info_log[info_log_size];

  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(shader_id, info_log_size, nullptr, info_log);

    switch (type)
    {
    case shader_type::vertex:
      std::cerr << "Vertex shader compilation error:\n"
                << info_log << std::endl;
      break;

    case shader_type::fragment:
      std::cerr << "Fragment shader compilation error:\n"
                << info_log << std::endl;
      break;

    default:
      FGE_ASSERT(0 && "Can not handle shader type");
    }
  }
}

static void check_link_errors(GLuint shader_id)
{
  GLint          success{};
  constexpr auto info_log_size = 1024;
  GLchar         info_log[info_log_size];

  glGetProgramiv(shader_id, GL_LINK_STATUS, &success);

  if (!success)
  {
    glGetProgramInfoLog(shader_id, info_log_size, nullptr, info_log);
    std::cerr << "Shader link error:\n" << info_log << std::endl;
  }
}

shader_program_handle
create_shader_program(const std::string &vertex_shader_source,
                      const std::string &fragment_shader_source)
{
  const auto vertex_shader_id           = glCreateShader(GL_VERTEX_SHADER);
  const auto vertex_shader_source_c_str = vertex_shader_source.c_str();
  glShaderSource(vertex_shader_id, 1, &vertex_shader_source_c_str, nullptr);
  glCompileShader(vertex_shader_id);
  check_compile_errors(vertex_shader_id, shader_type::vertex);

  const auto fragment_shader_id           = glCreateShader(GL_FRAGMENT_SHADER);
  const auto fragment_shader_source_c_str = fragment_shader_source.c_str();
  glShaderSource(fragment_shader_id, 1, &fragment_shader_source_c_str, nullptr);
  glCompileShader(fragment_shader_id);
  check_compile_errors(fragment_shader_id, shader_type::fragment);

  const auto shader_id = glCreateProgram();
  glAttachShader(shader_id, vertex_shader_id);
  glAttachShader(shader_id, fragment_shader_id);
  glLinkProgram(shader_id);
  check_link_errors(shader_id);

  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);

  const shader_program_handle handle = shaders.size();
  shader                      s;
  s.id = shader_id;
  shaders.push_back(s);

  return handle;
}

texture_handle create_texture(const void *data, const texture_info &info)
{
  GLuint texture_id{};
  glGenTextures(1, &texture_id);
  const auto target = to_gl(info.type);
  glBindTexture(target, texture_id);

  glTexParameteri(target, GL_TEXTURE_WRAP_S, to_gl(info.wrap_u));
  glTexParameteri(target, GL_TEXTURE_WRAP_T, to_gl(info.wrap_v));
  glTexParameteri(target,
                  GL_TEXTURE_MIN_FILTER,
                  to_gl(info.minification_filter));
  glTexParameteri(target,
                  GL_TEXTURE_MAG_FILTER,
                  to_gl(info.magnification_filter));

  glTexImage2D(target,
               0,
               to_gl(info.internal_format),
               info.width,
               info.height,
               0,
               to_gl(info.source_format),
               to_gl(info.source_type),
               data);

  if (info.generate_mipmap)
  {
    glGenerateMipmap(target);
  }

  glBindTexture(target, 0);

  const auto handle = textures.size();

  texture tex;
  tex.id   = texture_id;
  tex.type = target;
  textures.push_back(tex);

  return handle;
}

void set_uniform(const void *data)
{
  // Interpret to any type to read shader
  const auto real_data = static_cast<const set_uniform_command<int> *>(data);

  FGE_ASSERT(real_data->shader < shaders.size());

  auto &shader = shaders[real_data->shader];

  glUseProgram(shader.id);

  const auto location = glGetUniformLocation(shader.id, real_data->name);

  switch (real_data->type)
  {
  case uniform_type::mat4:
  {
    // Reinterpret to correct type
    const auto real_data =
        static_cast<const set_uniform_command<glm::mat4> *>(data);
    const auto mat = real_data->data;
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
  }
  break;

  case uniform_type::texture2d:
  {
    // Reinterpret to correct type
    const auto real_data =
        static_cast<const set_uniform_command<texture_handle> *>(data);
    const texture_handle handle = real_data->data;

    FGE_ASSERT(handle < textures.size());

    const auto tex = textures[handle];

    const auto free_texture_unit = shader.next_free_texture_slot;
    shader.next_free_texture_slot += 1;

    glActiveTexture(GL_TEXTURE0 + free_texture_unit);
    glBindTexture(tex.type, tex.id);
    glUniform1i(location, free_texture_unit);
  }
  break;

  default:
    FGE_ASSERT(0 && "Can not handle uniform type");
  }

  glUseProgram(0);
}

void draw(const void *data)
{
  const auto real_data = static_cast<const draw_command *>(data);

  FGE_ASSERT(real_data->shader < shaders.size());
  auto &shader = shaders[real_data->shader];

  FGE_ASSERT(real_data->vertices < vertex_buffers.size());
  const auto vertex_buffer = vertex_buffers[real_data->vertices];

  if (real_data->depth_test)
  {
    glEnable(GL_DEPTH_TEST);
  }
  else
  {
    glDisable(GL_DEPTH_TEST);
  }

  glUseProgram(shader.id);
  glBindVertexArray(vertex_buffer.vao);

  glDrawArrays(GL_TRIANGLES, 0, vertex_buffer.size);

  glUseProgram(0);
  glBindVertexArray(0);

  shader.next_free_texture_slot = 0;
}

void render_frame()
{
  const auto buckets = command_bucket::get_buckets();

  for (const auto bucket : buckets)
  {
    // TODO: Set state for bucket
    bucket->submit();
  }
}

void set_render_state(render_state state)
{
  GLuint gl_clear_state = 0;

  if ((state & render_state::clear_color) == render_state::clear_color)
  {
    gl_clear_state |= GL_COLOR_BUFFER_BIT;
  }
  if ((state & render_state::clear_depth) == render_state::clear_depth)
  {
    gl_clear_state |= GL_DEPTH_BUFFER_BIT;
  }

  glClear(gl_clear_state);
}

void set_clear_color(float red, float green, float blue, float alpha)
{
  glClearColor(red, green, blue, alpha);
}

} // namespace fge::gfx::gl

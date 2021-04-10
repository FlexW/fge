#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gfx/command_bucket.hpp"
#include "gfx/commands.hpp"
#include "gfx/graphic_backend.hpp"
#include "gfx/graphic_backend_context.hpp"
#include "gfx/graphic_backend_types.hpp"
#include "glfw_window.hpp"
#include "opengl_backend_types.hpp"
#include "opengl_backend_util.hpp"

namespace fge::gfx::gl
{

static std::vector<vertex_buffer> vertex_buffers;
static std::vector<texture>       textures;
static std::vector<shader>        shaders;
static std::vector<framebuffer>   framebuffers;
static std::vector<renderbuffer>  renderbuffers;

std::unique_ptr<window> create_window(broadcast *application_broadcast,
                                      const window_create_info &create_info)
{
  return std::make_unique<glfw::glfw_window>(application_broadcast,
                                             create_info);
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

  if (info.enable_border_color)
  {
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, info.border_color);
  }

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

renderbuffer_handle create_renderbuffer(const renderbuffer_info &info)
{
  GLuint renderbuffer_id{};
  glGenRenderbuffers(1, &renderbuffer_id);

  glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_id);

  glRenderbufferStorage(GL_RENDERBUFFER,
                        to_gl(info.internal_format),
                        info.width,
                        info.height);

  auto handle = renderbuffers.size();

  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  renderbuffer rb;
  rb.id = renderbuffer_id;
  renderbuffers.push_back(rb);

  return handle;
}

framebuffer_handle create_framebuffer(std::vector<attachment_info> attachments)
{
  GLuint framebuffer_id{};
  glGenFramebuffers(1, &framebuffer_id);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);

  GLuint color_attachment_count = 0;

  for (const auto attachment : attachments)
  {
    switch (attachment.target_type)
    {
    case render_target::renderbuffer:
    {
      FGE_ASSERT(attachment.handle < renderbuffers.size());
      const auto rb              = renderbuffers[attachment.handle];
      const auto renderbuffer_id = rb.id;

      switch (attachment.type)
      {
      case attachment_type::color:
      {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                  GL_COLOR_ATTACHMENT0 + color_attachment_count,
                                  GL_RENDERBUFFER,
                                  renderbuffer_id);
        ++color_attachment_count;
      }
      break;
      case attachment_type::depth:
      {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                  GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER,
                                  renderbuffer_id);
      }
      break;
      case attachment_type::stencil:
      {
        FGE_FAIL("Can not handle stencil attachment");
      }
      break;
      case attachment_type::depth_stencil:
      {
        FGE_FAIL("Can not handle depth stencil attachment");
      }
      break;
      }
    }
    break;
    case render_target::texture:
    {
      FGE_ASSERT(attachment.handle < textures.size());
      const auto tex        = textures[attachment.handle];
      const auto texture_id = tex.id;

      switch (attachment.type)
      {
      case attachment_type::color:
      {
        glFramebufferTexture(GL_FRAMEBUFFER,
                             GL_COLOR_ATTACHMENT0 + color_attachment_count,
                             texture_id,
                             0);
        ++color_attachment_count;
      }
      break;
      case attachment_type::depth:
      {
        glFramebufferTexture(GL_FRAMEBUFFER,
                             GL_DEPTH_ATTACHMENT,
                             texture_id,
                             0);
      }
      break;
      case attachment_type::stencil:
      {
        FGE_FAIL("Can not handle stencil attachment");
      }
      break;
      case attachment_type::depth_stencil:
      {
        FGE_FAIL("Can not handle depth stencil attachment");
      }
      break;
      }
    }
    break;
    }
  }

  if (attachments.size() == 1 && attachments[0].type == attachment_type::depth)
  {
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
  }

  FGE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) ==
             GL_FRAMEBUFFER_COMPLETE);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  const auto handle = framebuffers.size();

  struct framebuffer fb;
  fb.id = framebuffer_id;
  framebuffers.push_back(fb);

  return handle;
}

void dispatch_draw(const void *data)
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

void dispatch_set_uniform(const void *data)
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

  case uniform_type::vec3:
  {
    // Reinterpret to correct type
    const auto real_data =
        static_cast<const set_uniform_command<glm::vec3> *>(data);
    const auto vec = real_data->data;
    glUniform3fv(location, 1, glm::value_ptr(vec));
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

void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
  glViewport(x, y, width, height);
}

void bind_framebuffer(framebuffer_handle handle)
{
  FGE_ASSERT(handle < framebuffers.size());
  auto fb = framebuffers[handle];

  glBindFramebuffer(GL_FRAMEBUFFER, fb.id);
}

void render_frame()
{
  const auto buckets = gfx::command_bucket::get_buckets();

  for (const auto bucket : buckets)
  {
    // TODO: Set state for bucket
    bucket->submit();
  }
}

void bind_default_framebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
graphic_backend_context load_opengl_backend_context()
{
  graphic_backend_context context{};

  context.create_window            = create_window;
  context.create_vertex_buffer     = create_vertex_buffer;
  context.create_shader_program    = create_shader_program;
  context.create_texture           = create_texture;
  context.create_renderbuffer      = create_renderbuffer;
  context.create_framebuffer       = create_framebuffer;
  context.dispatch_draw            = dispatch_draw;
  context.dispatch_set_uniform     = dispatch_set_uniform;
  context.set_render_state         = set_render_state;
  context.set_clear_color          = set_clear_color;
  context.set_viewport             = set_viewport;
  context.bind_framebuffer         = bind_framebuffer;
  context.bind_default_framebuffer = bind_default_framebuffer;
  context.render_frame             = render_frame;

  return context;
}

} // namespace fge::gfx::gl

#include "gl_renderer.hpp"
#include "application.hpp"
#include "framebuffer.hpp"
#include "gl.hpp"
#include "graphic/render_info.hpp"
#include "graphic/renderbuffer.hpp"
#include "graphic/shader_preprocessor.hpp"
#include "graphic/texture.hpp"
#include "index_buffer.hpp"
#include "log/log.hpp"
#include "renderbuffer.hpp"
#include "shader.hpp"
#include "std.hpp"
#include "texture2d.hpp"
#include "util/assert.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"

namespace Fge::Gl
{

GLenum draw_mode_to_gl_draw_mode(DrawMode draw_mode)
{
  switch (draw_mode)
  {
  case DrawMode::LINES:
    return GL_LINES;

  case DrawMode::TRIANGLES:
    return GL_TRIANGLES;

  default:
    FGE_FAIL("No such draw mode");
  }
}

Renderer::Renderer() { glEnable(GL_DEPTH_TEST); }

void Renderer::begin_render() {}

void Renderer::end_render() {}

void Renderer::clear_color() { glClear(GL_COLOR_BUFFER_BIT); }

void Renderer::clear_depth() { glClear(GL_DEPTH_BUFFER_BIT); }

void Renderer::set_clear_color(float r, float g, float b, float a)
{
  glClearColor(r, g, b, a);
}

void Renderer::blit_framebuffer(int32_t srcx0,
                                int32_t srcy0,
                                int32_t srcx1,
                                int32_t srcy1,
                                int32_t dstx0,
                                int32_t dsty0,
                                int32_t dstx1,
                                int32_t dsty1)
{
  glBlitFramebuffer(srcx0,
                    srcy0,
                    srcx1,
                    srcy1,
                    dstx0,
                    dsty0,
                    dstx1,
                    dsty1,
                    GL_COLOR_BUFFER_BIT,
                    GL_NEAREST);
}

std::shared_ptr<Fge::VertexArray> Renderer::create_vertex_array()
{
  return std::make_shared<Gl::VertexArray>();
}

std::shared_ptr<Fge::IndexBuffer>
Renderer::create_index_buffer(const std::vector<uint32_t> &indices)
{
  return std::make_shared<Gl::IndexBuffer>(indices);
}

std::shared_ptr<Fge::VertexBufferPNTBT>
Renderer::create_vertex_buffer_pntbt(const std::vector<VertexPNTBT> &vertices)
{
  return std::make_shared<Gl::VertexBufferPNTBT>(vertices);
}

std::shared_ptr<Fge::VertexBufferP>
Renderer::create_vertex_buffer_p(const std::vector<VertexP> &vertices)
{
  return std::make_shared<Gl::VertexBufferP>(vertices);
}

std::shared_ptr<Fge::Texture2D>
Renderer::create_texture2d(const Texture2DConfig &config)
{
  return std::make_shared<Gl::Texture2D>(config);
}

std::shared_ptr<Fge::Renderbuffer>
Renderer::create_renderbuffer(const RenderbufferConfig &config)
{
  return std::make_shared<Gl::Renderbuffer>(config);
}

std::shared_ptr<Fge::Framebuffer>
Renderer::create_framebuffer_rrt(const FramebufferConfigRRT &config)
{
  return std::make_shared<Gl::FramebufferRRT>(config);
}

void Renderer::register_renderable(std::shared_ptr<RenderInfo> render_info)
{
  trace("Renderer", "Try to register renderable");

  for (auto renderable : renderables)
  {
    if (renderable == render_info)
    {
      return;
    }
  }

  trace("Renderer", "Register renderable");
  renderables.push_back(render_info);
}

void Renderer::unregister_renderable(std::shared_ptr<RenderInfo> render_info)
{
  trace("Renderer", "Try to unregister renderable");

  for (size_t i = 0; i < renderables.size(); ++i)
  {
    if (renderables[i] == render_info)
    {
      trace("Renderer", "Unregister renderable");
      renderables.erase(renderables.begin() + i);
    }
  }
}

const std::vector<std::shared_ptr<RenderInfo>> &
Renderer::get_renderables() const
{
  return renderables;
}

void Renderer::draw(const Fge::VertexArray &vertex_array,
                    const Fge::IndexBuffer &index_buffer,
                    Material &              material,
                    DrawMode                draw_mode)
{
  material.bind();
  vertex_array.bind();

  glDrawElements(draw_mode_to_gl_draw_mode(draw_mode),
                 index_buffer.get_count(),
                 GL_UNSIGNED_INT,
                 nullptr);

  vertex_array.unbind();
  material.unbind();
}

void Renderer::draw(const Fge::VertexArray &vertex_array,
                    Material &              material,
                    DrawMode                draw_mode)
{
  vertex_array.bind();
  material.bind();

  glDrawArrays(draw_mode_to_gl_draw_mode(draw_mode),
               0,
               vertex_array.get_count());

  material.unbind();
  vertex_array.unbind();
}

void Renderer::set_viewport(uint32_t x,
                            uint32_t y,
                            uint32_t width,
                            uint32_t height)
{
  glViewport(x, y, width, height);
}

std::shared_ptr<Fge::Shader>
Renderer::create_shader(const std::string &vertex_shader_filename,
                        const std::string &fragment_shader_filename,
                        const std::vector<std::string> &shader_defines)
{
  auto app          = Application::get_instance();
  auto file_manager = app->get_file_manager();

  auto vertex_shader_code = file_manager->read_text_file(
      (file_manager->get_shaders_path() / vertex_shader_filename).string());
  auto fragment_shader_code = file_manager->read_text_file(
      (file_manager->get_shaders_path() / fragment_shader_filename).string());

  ShaderPreprocessor shader_preprocessor(std::vector<std::string>{
      file_manager->get_shaders_path().string() + "/"});
  for (const auto &define : shader_defines)
  {
    shader_preprocessor.set_define(define);
  }

  // FIXME: This will fail if shaders are not in base directory
  vertex_shader_code = shader_preprocessor.preprocess(
      file_manager->get_shaders_path().string() + "/",
      vertex_shader_code);
  fragment_shader_code = shader_preprocessor.preprocess(
      file_manager->get_shaders_path().string() + "/",
      fragment_shader_code);

  return std::make_shared<Gl::Shader>(vertex_shader_code, fragment_shader_code);
}

void Renderer::register_point_light(std::shared_ptr<PointLight> point_light)
{
  trace("Renderer", "Try to register point light");

  for (auto l : point_lights)
  {
    if (l == point_light)
    {
      return;
    }
  }

  trace("Renderer", "Register point light");
  point_lights.push_back(point_light);
}

void Renderer::register_directional_light(
    std::shared_ptr<DirectionalLight> directional_light)
{
  trace("Renderer", "Try to register directional light");

  for (auto l : directional_lights)
  {
    if (l == directional_light)
    {
      return;
    }
  }

  trace("Renderer", "Register directional light");
  directional_lights.push_back(directional_light);
}

void Renderer::register_spot_light(std::shared_ptr<SpotLight> spot_light)
{
  trace("Renderer", "Try to register spot light");

  for (auto l : spot_lights)
  {
    if (l == spot_light)
    {
      return;
    }
  }

  trace("Renderer", "Register spot light");
  spot_lights.push_back(spot_light);
}

void Renderer::unregister_point_light(std::shared_ptr<PointLight> point_light)
{
  trace("Renderer", "Try to unregister point light");

  for (size_t i = 0; i < point_lights.size(); ++i)
  {
    if (point_lights[i] == point_light)
    {
      trace("Renderer", "Unregister point light");
      point_lights.erase(point_lights.begin() + i);
    }
  }
}

void Renderer::unregister_directional_light(
    std::shared_ptr<DirectionalLight> directional_light)
{
  trace("Renderer", "Try to unregister directional light");

  for (size_t i = 0; i < directional_lights.size(); ++i)
  {
    if (directional_lights[i] == directional_light)
    {
      trace("Renderer", "Unregister directional light");
      directional_lights.erase(directional_lights.begin() + i);
    }
  }
}

void Renderer::unregister_spot_light(std::shared_ptr<SpotLight> spot_light)
{
  trace("Renderer", "Try to unregister spot light");

  for (size_t i = 0; i < spot_lights.size(); ++i)
  {
    if (spot_lights[i] == spot_light)
    {
      trace("Renderer", "Unregister spot light");
      spot_lights.erase(spot_lights.begin() + i);
    }
  }
}

const std::vector<std::shared_ptr<PointLight>> &
Renderer::get_point_lights() const
{
  return point_lights;
}

const std::vector<std::shared_ptr<DirectionalLight>> &
Renderer::get_directional_lights() const
{
  return directional_lights;
}

const std::vector<std::shared_ptr<SpotLight>> &Renderer::get_spot_lights() const
{
  return spot_lights;
}

void Renderer::terminate()
{
  if (point_lights.size() > 0)
  {
    trace("Renderer",
          "Terminate cleared {} forgotten point lights",
          point_lights.size());
  }
  point_lights.clear();

  if (spot_lights.size() > 0)
  {
    trace("Renderer",
          "Terminate cleared {} forgotten spot lights",
          spot_lights.size());
  }
  spot_lights.clear();

  if (directional_lights.size() > 0)
  {
    trace("Renderer",
          "Terminate cleared {} forgotten directional lights",
          directional_lights.size());
  }
  directional_lights.clear();

  if (renderables.size() > 0)
  {
    trace("Renderer",
          "Terminate cleared {} forgotten renderables",
          renderables.size());
  }
  renderables.clear();
}

} // namespace Fge::Gl

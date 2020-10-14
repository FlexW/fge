#include "gl_renderer.hpp"
#include "application.hpp"
#include "gl.hpp"
#include "graphic/shader_preprocessor.hpp"
#include "index_buffer.hpp"
#include "log/log.hpp"
#include "shader.hpp"
#include "std.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"

namespace Fge::Gl
{

Renderer::Renderer() { glEnable(GL_DEPTH_TEST); }

void Renderer::begin_render() {}

void Renderer::end_render() {}

void Renderer::clear_color() { glClear(GL_COLOR_BUFFER_BIT); }

void Renderer::clear_depth() { glClear(GL_DEPTH_BUFFER_BIT); }

void Renderer::set_clear_color(float r, float g, float b, float a)
{
  glClearColor(r, g, b, a);
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
                    Material &              material)
{
  material.bind();
  vertex_array.bind();

  glDrawElements(GL_TRIANGLES,
                 index_buffer.get_count(),
                 GL_UNSIGNED_INT,
                 nullptr);

  vertex_array.unbind();
  material.unbind();
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
      file_manager->get_shaders_path() / vertex_shader_filename);
  auto fragment_shader_code = file_manager->read_text_file(
      file_manager->get_shaders_path() / fragment_shader_filename);

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

} // namespace Fge::Gl

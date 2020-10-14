#pragma once

#include "graphic/render_info.hpp"
#include "index_buffer.hpp"
#include "std.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer_pntbt.hpp"
#include "vertices.hpp"

namespace Fge
{

class Renderer
{
public:
  virtual ~Renderer() = default;

  virtual void begin_render() = 0;

  virtual void end_render() = 0;

  virtual void clear_color() = 0;

  virtual void clear_depth() = 0;

  virtual void set_clear_color(float r, float g, float b, float a) = 0;

  virtual std::shared_ptr<VertexArray> create_vertex_array() = 0;

  virtual std::shared_ptr<IndexBuffer>
  create_index_buffer(const std::vector<uint32_t> &indices) = 0;

  virtual std::shared_ptr<VertexBufferPNTBT>
  create_vertex_buffer_pntbt(const std::vector<VertexPNTBT> &vertices) = 0;

  virtual std::shared_ptr<Shader>
  create_shader(const std::string &             vertex_shader_filename,
                const std::string &             fragment_shader_filename,
                const std::vector<std::string> &shader_defines) = 0;

  virtual void register_renderable(std::shared_ptr<RenderInfo> render_info) = 0;

  virtual void
  unregister_renderable(std::shared_ptr<RenderInfo> render_info) = 0;

  virtual const std::vector<std::shared_ptr<RenderInfo>> &
  get_renderables() const = 0;

  virtual void draw(const VertexArray &vertex_array,
                    const IndexBuffer &index_buffer,
                    Material &         material) = 0;

  virtual void
  set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
};

} // namespace Fge

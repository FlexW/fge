#pragma once

#include "graphic/render_info.hpp"
#include "graphic/renderer.hpp"

namespace Fge::Gl
{

class Renderer : public Fge::Renderer
{
public:
  Renderer();

  void begin_render() override;

  void end_render() override;

  void clear_color() override;

  void clear_depth() override;

  void set_clear_color(float r, float g, float b, float a) override;

  std::shared_ptr<VertexArray> create_vertex_array() override;

  std::shared_ptr<IndexBuffer>
  create_index_buffer(const std::vector<uint32_t> &indices) override;

  std::shared_ptr<VertexBufferPNTBT>
  create_vertex_buffer_pntbt(const std::vector<VertexPNTBT> &vertices) override;

  std::shared_ptr<Shader>
  create_shader(const std::string &             vertex_shader_filename,
                const std::string &             fragment_shader_filename,
                const std::vector<std::string> &shader_defines) override;

  void register_renderable(std::shared_ptr<RenderInfo> render_info) override;

  void unregister_renderable(std::shared_ptr<RenderInfo> render_info) override;

  const std::vector<std::shared_ptr<RenderInfo>> &
  get_renderables() const override;

  void draw(const VertexArray &vertex_array,
            const IndexBuffer &index_buffer,
            Material &         material) override;

  void set_viewport(uint32_t x,
                    uint32_t y,
                    uint32_t width,
                    uint32_t height) override;

private:
  std::vector<std::shared_ptr<RenderInfo>> renderables;
};

} // namespace Fge::Gl

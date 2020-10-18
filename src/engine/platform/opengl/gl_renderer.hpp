#pragma once

#include "graphic/point_light.hpp"
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

  void blit_framebuffer(int32_t srcx0,
                        int32_t srcy0,
                        int32_t srcx1,
                        int32_t srcy1,
                        int32_t dstx0,
                        int32_t dsty0,
                        int32_t dstx1,
                        int32_t dsty1) override;

  std::shared_ptr<Fge::VertexArray> create_vertex_array() override;

  std::shared_ptr<Fge::IndexBuffer>
  create_index_buffer(const std::vector<uint32_t> &indices) override;

  std::shared_ptr<Fge::VertexBuffer>
  create_vertex_buffer(const std::vector<VertexPNTBT> &vertices) override;

  std::shared_ptr<Fge::VertexBuffer>
  create_vertex_buffer(const std::vector<VertexP> &vertices) override;

  std::shared_ptr<VertexBuffer>
  create_vertex_buffer(const std::vector<VertexPNTBBWT> &vertices) override;

  std::shared_ptr<Fge::Shader>
  create_shader(const std::string &             vertex_shader_filename,
                const std::string &             fragment_shader_filename,
                const std::vector<std::string> &shader_defines) override;

  std::shared_ptr<Fge::Texture2D>
  create_texture2d(const Texture2DConfig &config) override;

  std::shared_ptr<Fge::Renderbuffer>
  create_renderbuffer(const RenderbufferConfig &config) override;

  std::shared_ptr<Fge::Framebuffer>
  create_framebuffer_rrt(const FramebufferConfigRRT &config) override;

  void register_renderable(std::shared_ptr<RenderInfo> render_info) override;

  void unregister_renderable(std::shared_ptr<RenderInfo> render_info) override;

  const std::vector<std::shared_ptr<RenderInfo>> &
  get_renderables() const override;

  void draw(const Fge::VertexArray &vertex_array,
            const Fge::IndexBuffer &index_buffer,
            Material &              material,
            DrawMode                draw_mode) override;

  void draw(const Fge::VertexArray &vertex_array,
            Material &              material,
            DrawMode                draw_mode) override;

  void set_viewport(uint32_t x,
                    uint32_t y,
                    uint32_t width,
                    uint32_t height) override;

  void register_point_light(std::shared_ptr<PointLight> point_light) override;

  void register_directional_light(
      std::shared_ptr<DirectionalLight> directional_light) override;

  void register_spot_light(std::shared_ptr<SpotLight> spot_light) override;

  void unregister_point_light(std::shared_ptr<PointLight> point_light) override;

  void unregister_directional_light(
      std::shared_ptr<DirectionalLight> directional_light) override;

  void unregister_spot_light(std::shared_ptr<SpotLight> spot_light) override;

  const std::vector<std::shared_ptr<PointLight>> &
  get_point_lights() const override;

  const std::vector<std::shared_ptr<DirectionalLight>> &
  get_directional_lights() const override;

  const std::vector<std::shared_ptr<SpotLight>> &
  get_spot_lights() const override;

  void terminate() override;

private:
  std::vector<std::shared_ptr<RenderInfo>> renderables;

  std::vector<std::shared_ptr<PointLight>>       point_lights;
  std::vector<std::shared_ptr<SpotLight>>        spot_lights;
  std::vector<std::shared_ptr<DirectionalLight>> directional_lights;
};

} // namespace Fge::Gl

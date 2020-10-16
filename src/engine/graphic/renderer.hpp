#pragma once

#include "directional_light.hpp"
#include "graphic/framebuffer.hpp"
#include "graphic/render_info.hpp"
#include "graphic/renderbuffer.hpp"
#include "graphic/texture.hpp"
#include "index_buffer.hpp"
#include "point_light.hpp"
#include "spot_light.hpp"
#include "std.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer_p.hpp"
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

  virtual void blit_framebuffer(int32_t srcx0,
                                int32_t srcy0,
                                int32_t srcx1,
                                int32_t srcy1,
                                int32_t dstx0,
                                int32_t dsty0,
                                int32_t dstx1,
                                int32_t dsty1) = 0;

  virtual std::shared_ptr<VertexArray> create_vertex_array() = 0;

  virtual std::shared_ptr<IndexBuffer>
  create_index_buffer(const std::vector<uint32_t> &indices) = 0;

  virtual std::shared_ptr<VertexBufferPNTBT>
  create_vertex_buffer_pntbt(const std::vector<VertexPNTBT> &vertices) = 0;

  virtual std::shared_ptr<VertexBufferP>
  create_vertex_buffer_p(const std::vector<VertexP> &vertices) = 0;

  virtual std::shared_ptr<Shader>
  create_shader(const std::string &             vertex_shader_filename,
                const std::string &             fragment_shader_filename,
                const std::vector<std::string> &shader_defines) = 0;

  virtual std::shared_ptr<Texture2D>
  create_texture2d(const Texture2DConfig &config) = 0;

  virtual std::shared_ptr<Renderbuffer>
  create_renderbuffer(const RenderbufferConfig &config) = 0;

  virtual std::shared_ptr<Framebuffer>
  create_framebuffer_rrt(const FramebufferConfigRRT &config) = 0;

  virtual void register_renderable(std::shared_ptr<RenderInfo> render_info) = 0;

  virtual void
  unregister_renderable(std::shared_ptr<RenderInfo> render_info) = 0;

  virtual void
  register_point_light(std::shared_ptr<PointLight> point_light) = 0;

  virtual void register_directional_light(
      std::shared_ptr<DirectionalLight> directional_light) = 0;

  virtual void register_spot_light(std::shared_ptr<SpotLight> spot_light) = 0;

  virtual void
  unregister_point_light(std::shared_ptr<PointLight> point_light) = 0;

  virtual void unregister_directional_light(
      std::shared_ptr<DirectionalLight> directional_light) = 0;

  virtual void unregister_spot_light(std::shared_ptr<SpotLight> spot_light) = 0;

  virtual const std::vector<std::shared_ptr<RenderInfo>> &
  get_renderables() const = 0;

  virtual const std::vector<std::shared_ptr<PointLight>> &
  get_point_lights() const = 0;

  virtual const std::vector<std::shared_ptr<DirectionalLight>> &
  get_directional_lights() const = 0;

  virtual const std::vector<std::shared_ptr<SpotLight>> &
  get_spot_lights() const = 0;

  virtual void draw(const VertexArray &vertex_array,
                    const IndexBuffer &index_buffer,
                    Material &         material,
                    DrawMode           draw_mode) = 0;

  virtual void draw(const VertexArray &vertex_array,
                    Material &         material,
                    DrawMode           draw_mode) = 0;

  virtual void
  set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
};

} // namespace Fge

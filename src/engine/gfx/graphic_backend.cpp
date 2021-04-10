#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "backend/backends.hpp"
#include "graphic_backend.hpp"
#include "graphic_backend_context.hpp"
#include "util/assert.hpp"

namespace fge::gfx::graphic_backend
{

static std::unordered_map<std::string,
                          std::function<graphic_backend_context(void)>>
    registered_graphic_backends;

static graphic_backend_context context;

void init() {
  const std::string backend_to_load_name("opengl");

  auto graphic_backends_iter = graphic_backends.find(backend_to_load_name);
  if (graphic_backends_iter == registered_graphic_backends.end())
  {
    throw std::runtime_error("Can not load graphic backend: " +
                             backend_to_load_name);
  }

  context = graphic_backends_iter->second();
}

std::unique_ptr<window> create_window(broadcast *application_broadcast,
                                      const window_create_info &create_info)
{
  FGE_ASSERT(context.create_window);
  return context.create_window(application_broadcast, create_info);
}

vertex_buffer_handle create_vertex_buffer(const memory &              memory,
                                          const vertex_buffer_layout &layout)
{
  FGE_ASSERT(context.create_vertex_buffer);
  return context.create_vertex_buffer(memory, layout);
}

shader_program_handle
create_shader_program(const std::string &vertex_shader_source,
                      const std::string &fragment_shader_source)
{
  FGE_ASSERT(context.create_shader_program);
  return context.create_shader_program(vertex_shader_source,
                                       fragment_shader_source);
}

texture_handle create_texture(const void *data, const texture_info &info)
{
  FGE_ASSERT(context.create_texture);
  return context.create_texture(data, info);
}

renderbuffer_handle create_renderbuffer(const renderbuffer_info &info)
{
  FGE_ASSERT(context.create_renderbuffer);
  return context.create_renderbuffer(info);
}

framebuffer_handle create_framebuffer(std::vector<attachment_info> attachments)
{
  FGE_ASSERT(context.create_framebuffer);
  return context.create_framebuffer(attachments);
}

void dispatch_draw(const void *data)
{
  FGE_ASSERT(context.dispatch_draw);
  context.dispatch_draw(data);
}

void dispatch_set_uniform(const void *data)
{
  FGE_ASSERT(context.dispatch_set_uniform);
  context.dispatch_set_uniform(data);
}

void set_render_state(render_state state)
{
  FGE_ASSERT(context.set_render_state);
  context.set_render_state(state);
}

void set_clear_color(float red, float green, float blue, float alpha)
{
  FGE_ASSERT(context.set_clear_color);
  context.set_clear_color(red, green, blue, alpha);
}

void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
  FGE_ASSERT(context.set_viewport);
  context.set_viewport(x, y, width, height);
}

void bind_framebuffer(framebuffer_handle handle)
{
  FGE_ASSERT(context.bind_framebuffer);
  context.bind_framebuffer(handle);
}

void bind_default_framebuffer()
{
  FGE_ASSERT(context.bind_default_framebuffer);
  context.bind_default_framebuffer();
}

void render_frame()
{
  FGE_ASSERT(context.render_frame);
  context.render_frame();
}

} // namespace fge::gfx::graphic_backend

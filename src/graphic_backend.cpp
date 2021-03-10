#include "graphic_backend.hpp"
#include "gl_backend.hpp"

namespace fge::gfx
{

vertex_buffer_layout_element::vertex_buffer_layout_element(uint8_t        count,
                                                           attribute_type type)
    : count(count),
      type(type)
{
}

vertex_buffer_layout &vertex_buffer_layout::add_float(uint8_t count)
{
  elements.emplace_back(count, attribute_type::floating);
  return *this;
}

std::vector<vertex_buffer_layout_element>
vertex_buffer_layout::get_elements() const
{
  return elements;
}

vertex_buffer_handle create_vertex_buffer(const memory &              memory,
                                          const vertex_buffer_layout &layout)
{
  return gl::create_vertex_buffer(memory, layout);
}

shader_program_handle
create_shader_program(const std::string &vertex_shader_source,
                      const std::string &fragment_shader_source)
{
  return gl::create_shader_program(vertex_shader_source,
                                   fragment_shader_source);
}

texture_handle create_texture(const void *data, const texture_info &info)
{
  return gl::create_texture(data, info);
}

renderbuffer_handle create_renderbuffer(const renderbuffer_info &info)
{
  return gl::create_renderbuffer(info);
}

framebuffer_handle create_framebuffer(std::vector<attachment_info> attachments)
{
  return gl::create_framebuffer(attachments);
}

void draw(const void *data) { gl::draw(data); }

void set_uniform(const void *data) { gl::set_uniform(data); }

void render_frame() { gl::render_frame(); }

void set_render_state(render_state state) { gl::set_render_state(state); }

void set_clear_color(float red, float green, float blue, float alpha)
{
  gl::set_clear_color(red, green, blue, alpha);
}

void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
  gl::set_viewport(x, y, width, height);
}

void bind_framebuffer(framebuffer_handle handle)
{
  gl::bind_framebuffer(handle);
}

void bind_default_framebuffer() { gl::bind_default_framebuffer(); }

} // namespace fge::gfx

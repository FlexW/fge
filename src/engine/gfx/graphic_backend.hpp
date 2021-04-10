#pragma once

#include <functional>
#include <memory>
#include <string>

#include "gfx/window.hpp"
#include "graphic_backend_context.hpp"
#include "graphic_backend_types.hpp"

namespace fge::gfx::graphic_backend
{

void init();

std::unique_ptr<window> create_window(broadcast *application_broadcast,
                                      const window_create_info &create_info);

vertex_buffer_handle create_vertex_buffer(const memory &              memory,
                                          const vertex_buffer_layout &layout);

shader_program_handle
create_shader_program(const std::string &vertex_shader_source,
                      const std::string &fragment_shader_source);

texture_handle create_texture(const void *data, const texture_info &info);

renderbuffer_handle create_renderbuffer(const renderbuffer_info &info);

framebuffer_handle create_framebuffer(std::vector<attachment_info> attachments);

void dispatch_draw(const void *data);

void dispatch_set_uniform(const void *data);

void set_render_state(render_state state);

void set_clear_color(float red, float green, float blue, float alpha);

void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

void bind_framebuffer(framebuffer_handle handle);

void bind_default_framebuffer();

void render_frame();

} // namespace fge::gfx::graphic_backend

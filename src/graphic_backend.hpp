#pragma once

#include <string>
#include <vector>

#include "graphic_backend_types.hpp"

namespace fge::gfx
{

class vertex_buffer_layout_element
{
public:
  const uint8_t       count;
  const attribute_type type;

  vertex_buffer_layout_element(uint8_t count, attribute_type type);
};

class vertex_buffer_layout
{
public:
  vertex_buffer_layout &add_float(uint8_t count);

  std::vector<vertex_buffer_layout_element> get_elements() const;

private:
  std::vector<vertex_buffer_layout_element> elements;
};

vertex_buffer_handle create_vertex_buffer(const memory &              memory,
                                          const vertex_buffer_layout &layout);

shader_program_handle
create_shader_program(const std::string &vertex_shader_source,
                      const std::string &fragment_shader_source);

texture_handle create_texture(const void *data, const texture_info &info);

void draw(const void *data);

void set_uniform(const void *data);

void render_frame();

void set_render_state(render_state state);

void set_clear_color(float red, float green, float blue, float alpha);

} // namespace fge::gfx

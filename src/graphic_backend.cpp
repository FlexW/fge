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

vertex_buffer create_vertex_buffer(const memory &              memory,
                                   const vertex_buffer_layout &layout)
{
  return gl::create_vertex_buffer(memory, layout);
}

index_buffer create_index_buffer(const memory &memory)
{
  return gl::create_index_buffer(memory);
}

} // namespace fge::gfx

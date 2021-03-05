#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace fge::gfx
{

using vertex_buffer_layout_handle = uint32_t;
using vertex_buffer_handle        = uint32_t;
using index_buffer_handle         = uint32_t;

struct memory
{
  void *      data{};
  std::size_t size;
};

enum class attribute_type
{
  floating
};

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

struct vertex_buffer
{
  vertex_buffer_handle        handle{};
  vertex_buffer_layout_handle layout_handle{};
};

struct index_buffer
{
  index_buffer_handle handle{};
};

struct draw_command
{
  vertex_buffer vertices;
  index_buffer  indices;
};

vertex_buffer create_vertex_buffer(const memory &              memory,
                                   const vertex_buffer_layout &layout);

index_buffer create_index_buffer(const memory &memory);

void draw(const draw_command &draw_command);

} // namespace fge::gfx

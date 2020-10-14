#pragma once

#include "graphic/vertex_buffer_layout.hpp"

namespace Fge::Gl
{

class VertexBufferLayoutElement : public Fge::VertexBufferLayoutElement
{
public:
  VertexBufferLayoutElement(size_t size, int32_t type, uint32_t normalized);

  uint32_t get_size_of_type(int32_t type) override;
};

class VertexBufferLayout : public Fge::VertexBufferLayout
{
public:
  void push_float(std::size_t size) override;

  void push_uint(std::size_t size) override;

  void push_int(std::size_t size) override;
};

} // namespace Fge::Gl

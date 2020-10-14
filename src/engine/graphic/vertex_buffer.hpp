#pragma once

#include "std.hpp"
#include "vertex_buffer_layout.hpp"

namespace Fge
{

class VertexBuffer
{
public:
  VertexBuffer() = default;

  virtual ~VertexBuffer() = default;

  virtual const VertexBufferLayout &get_layout() const = 0;

  virtual void bind() = 0;

  virtual void unbind() = 0;

  virtual uint32_t get_count() const = 0;

private:
  VertexBuffer(const VertexBuffer &other) = delete;

  void operator=(const VertexBuffer &other) = delete;
};

} // namespace Fge

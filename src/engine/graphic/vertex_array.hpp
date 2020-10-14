#pragma once

#include "index_buffer.hpp"
#include "std.hpp"
#include "vertex_buffer.hpp"

namespace Fge
{

class VertexArray
{
public:
  VertexArray() = default;

  virtual ~VertexArray() = default;

  virtual void add_buffer(std::shared_ptr<VertexBuffer> vertex_buffer) = 0;

  virtual void add_buffer(std::shared_ptr<IndexBuffer> index_buffer) = 0;

  virtual void bind() const = 0;

  virtual void unbind() const = 0;

  virtual uint32_t get_count() const = 0;

private:
  VertexArray(const VertexArray &other) = delete;

  void operator=(const VertexArray &other) = delete;
};

} // namespace Fge

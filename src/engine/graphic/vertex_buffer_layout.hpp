#pragma once

#include "std.hpp"

namespace Fge
{

class VertexBufferLayoutElement
{

public:
  virtual ~VertexBufferLayoutElement() = default;

  VertexBufferLayoutElement(std::size_t size, int type, unsigned int normalized)
      : size(size),
        type(type),
        normalized(normalized)
  {
  }

  size_t get_size() const { return size; }

  int32_t get_type() const { return type; }

  int32_t get_normalized() const { return normalized; }

  virtual uint32_t get_size_of_type(int32_t type) = 0;

protected:
  size_t   size       = 0;
  uint32_t type       = 0;
  uint32_t normalized = 0;
};

class VertexBufferLayout
{
public:
  virtual ~VertexBufferLayout() = default;

  const std::vector<std::shared_ptr<VertexBufferLayoutElement>> &
  get_elements() const
  {
    return elements;
  }

  size_t get_stride() const { return stride; }

  virtual void push_float(std::size_t size) = 0;

  virtual void push_uint(std::size_t size) = 0;

  virtual void push_int(std::size_t size) = 0;

  void set_stride(const std::size_t stride) { this->stride = stride; }

protected:
  std::size_t                            stride = 0;
  std::vector<std::shared_ptr<VertexBufferLayoutElement>> elements;
};

} // namespace Fge

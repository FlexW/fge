#pragma once

#include "std.hpp"

namespace Fge
{

class IndexBuffer
{
public:
  IndexBuffer(const std::vector<uint32_t> & /*indices*/) {}

  virtual ~IndexBuffer() = default;

  virtual void bind() const = 0;

  virtual void unbind() const = 0;

  virtual uint32_t get_count() const = 0;

private:
  IndexBuffer(const IndexBuffer &) = delete;

  void operator=(const IndexBuffer &) = delete;
};

} // namespace Fge

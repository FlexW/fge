#pragma once

#include "graphic/index_buffer.hpp"

namespace Fge::Gl
{

class IndexBuffer : public Fge::IndexBuffer
{
public:
  IndexBuffer(const std::vector<uint32_t> &indices);

  ~IndexBuffer();

  void bind() const override;

  void unbind() const override;

  uint32_t get_count() const override;

private:
  uint32_t                    id = 0;
  const std::vector<uint32_t> indices;
};

} // namespace Fge::Gl

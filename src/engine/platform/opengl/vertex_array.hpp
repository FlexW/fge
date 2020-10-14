#pragma once

#include "graphic/index_buffer.hpp"
#include "graphic/vertex_array.hpp"
#include "graphic/vertex_buffer.hpp"

namespace Fge::Gl
{

class VertexArray : public Fge::VertexArray
{
public:
  VertexArray();

  ~VertexArray();

  void add_buffer(std::shared_ptr<Fge::VertexBuffer> vertex_buffer) override;

  void add_buffer(std::shared_ptr<Fge::IndexBuffer> index_buffer) override;

  void bind() const override;

  void unbind() const override;

  uint32_t get_count() const override;

private:
  uint32_t id    = 0;
  uint32_t count = 0;

  uint32_t index = 0;

  std::vector<std::shared_ptr<Fge::VertexBuffer>> vertex_buffers;
  std::vector<std::shared_ptr<Fge::IndexBuffer>>  index_buffers;
};

} // namespace Fge::Gl

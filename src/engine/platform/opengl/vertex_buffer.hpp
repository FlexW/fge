#pragma once

#include "graphic/vertex_buffer_layout.hpp"
#include "graphic/vertex_buffer_pntbt.hpp"
#include "graphic/vertices.hpp"
#include "vertex_buffer_layout.hpp"

namespace Fge::Gl
{

class VertexBufferPNTBT : public Fge::VertexBufferPNTBT
{
public:
  VertexBufferPNTBT(const std::vector<VertexPNTBT> &vertices);

  ~VertexBufferPNTBT();

  const Fge::VertexBufferLayout &get_layout() const override;

  void bind() override;

  void unbind() override;

  uint32_t get_count() const override;

private:
  uint32_t id    = 0;
  uint32_t count = 0;

  VertexBufferLayout vertex_buffer_layout;
};

} // namespace Fge::Gl

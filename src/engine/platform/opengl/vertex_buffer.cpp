#include "vertex_buffer.hpp"
#include "gl.hpp"
#include "log/log.hpp"

namespace Fge::Gl
{

VertexBufferPNTBT::VertexBufferPNTBT(const std::vector<VertexPNTBT> &vertices)
    : Fge::VertexBufferPNTBT(vertices),
      count(vertices.size())
{
  glGenBuffers(1, &id);
  trace("VertexBuffer", "Created vertex buffer with id: {}", id);
  glBindBuffer(GL_ARRAY_BUFFER, id);

  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(VertexPNTBT),
               vertices.data(),
               GL_STATIC_DRAW);

  vertex_buffer_layout.push_float(3);
  vertex_buffer_layout.push_float(3);
  vertex_buffer_layout.push_float(3);
  vertex_buffer_layout.push_float(3);
  vertex_buffer_layout.push_float(2);
}

VertexBufferPNTBT::~VertexBufferPNTBT()
{
  trace("VertexBuffer", "Delete vertex buffer with id: {}", id);
  glDeleteBuffers(1, &id);
}

const Fge::VertexBufferLayout &VertexBufferPNTBT::get_layout() const
{
  return vertex_buffer_layout;
}

void VertexBufferPNTBT::bind() { glBindBuffer(GL_ARRAY_BUFFER, id); }

void VertexBufferPNTBT::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

uint32_t VertexBufferPNTBT::get_count() const { return count; }

} // namespace Fge::Gl

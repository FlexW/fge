#include "index_buffer.hpp"
#include "gl.hpp"
#include "log/log.hpp"

namespace Fge::Gl
{

IndexBuffer::IndexBuffer(const std::vector<uint32_t> &indices)
    : Fge::IndexBuffer(indices),
      indices(indices)
{
  glGenBuffers(1, &id);
  trace("IndexBuffer", "Created index buffer with id: {}", id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               get_count() * sizeof(unsigned),
               &indices[0],
               GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
  trace("IndexBuffer", "Delete index buffer with id: {}", id);
  glDeleteBuffers(1, &id);
}

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }

void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

uint32_t IndexBuffer::get_count() const { return indices.size(); }

} // namespace Fge::Gl

#include "vertex_buffer.hpp"
#include "gl.hpp"
#include "log/log.hpp"

namespace Fge::Gl
{

VertexBufferPNTBT::VertexBufferPNTBT(const std::vector<VertexPNTBT> &vertices)
    : count(vertices.size())
{
  glGenBuffers(1, &id);
  trace("VertexBufferPNTBT", "Created vertex buffer with id: {}", id);
  glBindBuffer(GL_ARRAY_BUFFER, id);

  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(VertexPNTBT),
               vertices.data(),
               GL_STATIC_DRAW);

  vertex_buffer_layout.push_float(3); // position
  vertex_buffer_layout.push_float(3); // normal
  vertex_buffer_layout.push_float(3); // tangent
  vertex_buffer_layout.push_float(3); // bitangent
  vertex_buffer_layout.push_float(2); // tex_coord
}

VertexBufferPNTBT::~VertexBufferPNTBT()
{
  trace("VertexBufferPNTBT", "Delete vertex buffer with id: {}", id);
  glDeleteBuffers(1, &id);
}

const Fge::VertexBufferLayout &VertexBufferPNTBT::get_layout() const
{
  return vertex_buffer_layout;
}

void VertexBufferPNTBT::bind() { glBindBuffer(GL_ARRAY_BUFFER, id); }

void VertexBufferPNTBT::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

uint32_t VertexBufferPNTBT::get_count() const { return count; }

VertexBufferPNTBBWT::VertexBufferPNTBBWT(
    const std::vector<VertexPNTBBWT> &vertices)
    : count(vertices.size())
{
  glGenBuffers(1, &id);
  trace("VertexBufferPNTBBWT", "Created vertex buffer with id: {}", id);
  glBindBuffer(GL_ARRAY_BUFFER, id);

  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(VertexPNTBBWT),
               vertices.data(),
               GL_STATIC_DRAW);

  vertex_buffer_layout.push_float(3); // position
  vertex_buffer_layout.push_float(3); // normal
  vertex_buffer_layout.push_float(3); // tangent
  vertex_buffer_layout.push_float(3); // bitangent
  vertex_buffer_layout.push_int(4);   // skin_bones
  vertex_buffer_layout.push_float(4); // skin_weights
  vertex_buffer_layout.push_float(2); // tex_coord
}

VertexBufferPNTBBWT::~VertexBufferPNTBBWT()
{
  trace("VertexBufferPNTBBWT", "Delete vertex buffer with id: {}", id);
  glDeleteBuffers(1, &id);
}

const Fge::VertexBufferLayout &VertexBufferPNTBBWT::get_layout() const
{
  return vertex_buffer_layout;
}

void VertexBufferPNTBBWT::bind() { glBindBuffer(GL_ARRAY_BUFFER, id); }

void VertexBufferPNTBBWT::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

uint32_t VertexBufferPNTBBWT::get_count() const { return count; }

VertexBufferP::VertexBufferP(const std::vector<VertexP> &vertices)
    : count(vertices.size())
{
  glGenBuffers(1, &id);
  trace("VertexBufferP", "Created vertex buffer with id: {}", id);
  glBindBuffer(GL_ARRAY_BUFFER, id);

  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(VertexP),
               vertices.data(),
               GL_STATIC_DRAW);

  vertex_buffer_layout.push_float(3); // position
}

VertexBufferP::~VertexBufferP()
{
  trace("VertexBufferP", "Delete vertex buffer with id: {}", id);
  glDeleteBuffers(1, &id);
}

const Fge::VertexBufferLayout &VertexBufferP::get_layout() const
{
  return vertex_buffer_layout;
}

void VertexBufferP::bind() { glBindBuffer(GL_ARRAY_BUFFER, id); }

void VertexBufferP::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

uint32_t VertexBufferP::get_count() const { return count; }

} // namespace Fge::Gl

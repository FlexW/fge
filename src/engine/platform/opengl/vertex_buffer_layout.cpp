#include "vertex_buffer_layout.hpp"
#include "gl.hpp"
#include "graphic/vertex_buffer_layout.hpp"
#include "util/assert.hpp"

namespace Fge::Gl
{

VertexBufferLayoutElement::VertexBufferLayoutElement(size_t   size,
                                                     int32_t  type,
                                                     uint32_t normalized)
    : Fge::VertexBufferLayoutElement(size, type, normalized)
{
}

uint32_t VertexBufferLayoutElement::get_size_of_type(int32_t type)
{
  switch (type)
  {
  case GL_FLOAT:
    return sizeof(GL_FLOAT);
  case GL_UNSIGNED_INT:
    return sizeof(GL_UNSIGNED_INT);
  case GL_INT:
    return sizeof(GL_INT);
  default:
    FGE_FAIL("No such type");
  }
}

void VertexBufferLayout::push_float(std::size_t size)
{
  auto element =
      std::make_shared<VertexBufferLayoutElement>(size, GL_FLOAT, GL_FALSE);
  elements.push_back(element);
  stride += element->get_size_of_type(GL_FLOAT) * size;
}

void VertexBufferLayout::push_uint(std::size_t size)
{
  auto element = std::make_shared<VertexBufferLayoutElement>(size,
                                                             GL_UNSIGNED_INT,
                                                             GL_FALSE);
  elements.push_back(element);
  stride += element->get_size_of_type(GL_UNSIGNED_INT) * size;
}

void VertexBufferLayout::push_int(std::size_t size)
{
  auto element =
      std::make_shared<VertexBufferLayoutElement>(size, GL_INT, GL_FALSE);
  elements.push_back(element);
  stride += element->get_size_of_type(GL_INT) * size;
}

} // namespace Fge::Gl

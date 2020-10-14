#include "vertex_array.hpp"
#include "gl.hpp"
#include "log/log.hpp"
#include "util/assert.hpp"

namespace Fge::Gl
{

VertexArray::VertexArray() { glGenVertexArrays(1, &id); }

VertexArray::~VertexArray()
{
  trace("VertexArray", "Delete vertex array with id: {}", id);
  glDeleteVertexArrays(1, &id);
}

void VertexArray::add_buffer(std::shared_ptr<Fge::VertexBuffer> vertex_buffer)
{
  size_t offset = 0;

  bind();
  vertex_buffer->bind();
  const auto &layout   = vertex_buffer->get_layout();
  const auto &elements = layout.get_elements();

  for (uint32_t i = 0; i < elements.size(); ++i)
  {
    const auto &element = elements[i];

    glEnableVertexAttribArray(index);

    auto size       = element->get_size();
    auto type       = element->get_type();
    auto normalized = element->get_normalized();
    auto stride     = layout.get_stride();

    trace("VertexArray",
          "Create vertex attribute pointer with index: {}, size: {}, , "
          "type: {}, normalized: {}, stride: {}, offset: {}",
          index,
          size,
          type,
          normalized,
          stride,
          offset);

    switch (elements[i]->get_type())
    {
    case GL_FLOAT:
      glVertexAttribPointer(index,
                            size,
                            type,
                            normalized,
                            stride,
                            reinterpret_cast<const void *>(offset));
      break;
    case GL_INT:
      glVertexAttribIPointer(index,
                             size,
                             GL_INT,
                             stride,
                             reinterpret_cast<const void *>(offset));
      break;

    case GL_UNSIGNED_INT:
      glVertexAttribIPointer(index,
                             size,
                             GL_UNSIGNED_INT,
                             stride,
                             reinterpret_cast<const void *>(offset));
      break;
    default:
      FGE_FAIL("No such type");
    }

    offset +=
        element->get_size() * element->get_size_of_type(element->get_type());

    ++index;
  }
  count += vertex_buffer->get_count();
  vertex_buffer->unbind();
  unbind();

  vertex_buffers.push_back(vertex_buffer);
}

void VertexArray::add_buffer(std::shared_ptr<Fge::IndexBuffer> index_buffer)
{
  bind();
  index_buffer->bind();
  unbind();

  index_buffers.push_back(index_buffer);
}

void VertexArray::bind() const { glBindVertexArray(id); }

void VertexArray::unbind() const { glBindVertexArray(0); }

uint32_t VertexArray::get_count() const { return count; }

} // namespace Fge::Gl

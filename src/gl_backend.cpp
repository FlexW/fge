#include <cassert>
#include <glad/glad.h>

#include "gl_backend.hpp"

namespace fge::gfx::gl
{

static std::vector<vertex_buffer> vertex_buffers;

struct gl_type_info
{
  GLuint      type;
  std::size_t size;
};

static gl_type_info get_gl_type_info(attribute_type type)
{
  std::size_t real_type_size;
  GLuint      real_type;

  switch (type)
  {
  case attribute_type::floating:
    real_type      = GL_FLOAT;
    real_type_size = sizeof(GL_FLOAT);
    break;
  default:
    assert(0 && "Can not handle type");
  }

  gl_type_info info{};
  info.type = real_type;
  info.size = real_type_size;

  return info;
}

vertex_buffer create_vertex_buffer(const memory &              memory,
                                   const vertex_buffer_layout &layout)
{
  GLuint vbo{};
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, memory.size, memory.data, GL_STATIC_DRAW);

  GLuint vao{};
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);

  // Calculate stride
  std::size_t stride = 0;
  for (const auto &element : layout.get_elements())
  {
    const auto gl_type = get_gl_type_info(element.type);
    stride += gl_type.size * element.count;
  }

  std::size_t offset     = 0;
  std::size_t index      = 0;
  const bool  normalized = false;

  for (const auto &element : layout.get_elements())
  {
    const auto gl_type = get_gl_type_info(element.type);

    glEnableVertexAttribArray(index);

    switch (gl_type.type)
    {
    case GL_FLOAT:
      glVertexAttribPointer(index,
                            element.count,
                            gl_type.type,
                            normalized,
                            stride,
                            reinterpret_cast<const void *>(offset));
      break;
    case GL_INT:
      glVertexAttribIPointer(index,
                             element.count,
                             GL_INT,
                             stride,
                             reinterpret_cast<const void *>(offset));
      break;

    case GL_UNSIGNED_INT:
      glVertexAttribIPointer(index,
                             element.count,
                             GL_UNSIGNED_INT,
                             stride,
                             reinterpret_cast<const void *>(offset));
      break;
    default:
      assert(0 && "Can not handle type");
    };

    offset += element.count * gl_type.size;
    ++index;
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  vertex_buffer vertex_buffer;
  vertex_buffer.handle                      = vbo;
  vertex_buffer.layout_handle               = vao;

  vertex_buffers.push_back(vertex_buffer);

  return vertex_buffer;
}

index_buffer create_index_buffer(const memory & /*memory*/) { return {}; }

} // namespace fge::gfx::gl

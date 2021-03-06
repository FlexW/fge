#pragma once

#include "graphic/index_buffer.hpp"
#include "graphic/material.hpp"
#include "graphic/vertex_array.hpp"

namespace Fge
{

enum class DrawMode
{
  TRIANGLES,
  LINES
};

class RenderInfo
{
public:
  RenderInfo(std::shared_ptr<VertexArray> vertex_array,
             std::shared_ptr<IndexBuffer> index_buffer,
             std::shared_ptr<Material>    material);

  RenderInfo(std::shared_ptr<VertexArray> vertex_array,
             std::shared_ptr<Material>    material);

  std::shared_ptr<VertexArray> get_vertex_array() { return vertex_array; }

  std::shared_ptr<IndexBuffer> get_index_buffer() { return index_buffer; }

  std::shared_ptr<Material> get_material() { return material; }

  void set_draw_mode(DrawMode draw_mode) { this->draw_mode = draw_mode; }

  DrawMode get_draw_mode() const { return draw_mode; }

private:
  std::shared_ptr<VertexArray>  vertex_array{};
  std::shared_ptr<IndexBuffer>  index_buffer{};
  std::shared_ptr<Material>     material{};

  DrawMode draw_mode = DrawMode::TRIANGLES;
};

} // namespace Fge

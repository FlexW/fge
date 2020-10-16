#pragma once

#include "graphic/material.hpp"
#include "render_info.hpp"
#include "std.hpp"
#include "unlit_material.hpp"
#include "vertex_array.hpp"
#include "vertices.hpp"

namespace Fge
{

class Grid
{
public:
  Grid(int32_t size);

  std::shared_ptr<VertexArray> get_vertex_array() { return vertex_array; }

  std::shared_ptr<Material> get_material() { return material; }

  std::shared_ptr<RenderInfo> create_render_info();

private:
  std::shared_ptr<VertexArray> vertex_array{};

  std::shared_ptr<UnlitMaterial> material{};
};

} // namespace Fge

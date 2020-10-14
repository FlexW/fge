#include "render_info.hpp"

namespace Fge
{

RenderInfo::RenderInfo(std::shared_ptr<VertexArray> vertex_array,
                       std::shared_ptr<IndexBuffer> index_buffer,
                       std::shared_ptr<Material>    material)
    : vertex_array(vertex_array),
      index_buffer(index_buffer),
      material(material)
{
}

} // namespace Fge

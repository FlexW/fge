#pragma once

#include "graphic/vertices.hpp"
#include "vertex_buffer.hpp"

namespace Fge
{

class VertexBufferP : public VertexBuffer
{
public:
  VertexBufferP(const std::vector<VertexP> & /*vertices*/) {}
};

} // namespace Fge

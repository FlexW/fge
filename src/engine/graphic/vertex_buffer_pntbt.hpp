#pragma once

#include "graphic/vertices.hpp"
#include "vertex_buffer.hpp"

namespace Fge
{

class VertexBufferPNTBT : public VertexBuffer
{
public:
  VertexBufferPNTBT(const std::vector<VertexPNTBT> & /*vertices*/) {}
};

} // namespace Fge

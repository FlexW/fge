#pragma once

#include "render_path.hpp"

namespace Fge
{

class ForwardRenderPath : public RenderPath
{
public:
  void render(const glm::mat4 &projection_mat, const Camera &camera) override;
};

} // namespace Fge

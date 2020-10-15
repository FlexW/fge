#pragma once

#include "render_path.hpp"

namespace Fge
{

class ForwardRenderPath : public RenderPath
{
public:
  void render(const glm::mat4 &projection_mat,
              const Camera &   camera,
              uint32_t         width,
              uint32_t         height) override;
};

} // namespace Fge

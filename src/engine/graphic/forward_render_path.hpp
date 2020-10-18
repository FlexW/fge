#pragma once

#include "graphic/directional_light.hpp"
#include "graphic/material.hpp"
#include "graphic/point_light.hpp"
#include "render_path.hpp"

namespace Fge
{

class ForwardRenderPath : public RenderPath
{
public:
  void render(const glm::mat4 & projection_mat,
              const CameraInfo &camera_info,
              uint32_t          width,
              uint32_t          height) override;

private:
  void set_lightning_uniforms(
      Material &                                      material,
      const std::vector<std::shared_ptr<PointLight>> &point_lights);

  void set_lightning_uniforms(
      Material &                                            material,
      const std::vector<std::shared_ptr<DirectionalLight>> &directional_lights);
};

} // namespace Fge

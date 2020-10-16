#pragma once

#include "graphic/point_light.hpp"
#include "scene/component.hpp"

namespace Fge
{

class PointLightComponent : public Component
{
public:
  PointLightComponent(
      Actor *            owner,
      int                update_order = 100,
      const std::string &type_name    = "Fge::PointLightComponent");

  ~PointLightComponent();

  void set_ambient_color(const glm::vec3 &ambient_color);

  void set_diffuse_color(const glm::vec3 &diffuse_color);

  void set_specular_color(const glm::vec3 &specular_color);

protected:
  void create() override;

private:
  std::shared_ptr<PointLight> point_light{};
};

} // namespace Fge

#pragma once

#include "graphic/directional_light.hpp"
#include "scene/component.hpp"

namespace Fge
{

class DirectionalLightComponent : public Component
{
public:
  DirectionalLightComponent(
      Actor *            owner,
      int                update_order = 100,
      const std::string &type_name    = "Fge::DirectionalLightComponent");

  ~DirectionalLightComponent();

  void set_ambient_color(const glm::vec3 &ambient_color);

  void set_diffuse_color(const glm::vec3 &diffuse_color);

  void set_specular_color(const glm::vec3 &specular_color);

  void set_direction(const glm::vec3 &direction);

protected:
  void create() override;

private:
  std::shared_ptr<DirectionalLight> directional_light{};
};

} // namespace Fge

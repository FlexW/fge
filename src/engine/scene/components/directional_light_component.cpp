#include "directional_light_component.hpp"
#include "application.hpp"
#include "scene/component.hpp"
#include "util/assert.hpp"

namespace Fge
{

DirectionalLightComponent::DirectionalLightComponent(
    Actor *            owner,
    int                update_order,
    const std::string &type_name)
    : Component(owner, update_order, type_name)
{
  directional_light = std::make_shared<DirectionalLight>();
}

DirectionalLightComponent::~DirectionalLightComponent()
{
  if (directional_light)
  {
    auto app      = Application::get_instance();
    auto renderer = app->get_graphic_manager()->get_renderer();
    renderer->unregister_directional_light(directional_light);
  }
}

void DirectionalLightComponent::create()
{
  FGE_ASSERT(owner);

  auto app      = Application::get_instance();
  auto renderer = app->get_graphic_manager()->get_renderer();

  renderer->register_directional_light(directional_light);
}

void DirectionalLightComponent::set_ambient_color(
    const glm::vec3 &ambient_color)
{
  directional_light->ambient_color = ambient_color;
}

void DirectionalLightComponent::set_diffuse_color(
    const glm::vec3 &diffuse_color)
{
  directional_light->diffuse_color = diffuse_color;
}

void DirectionalLightComponent::set_specular_color(
    const glm::vec3 &specular_color)
{
  directional_light->specular_color = specular_color;
}

void DirectionalLightComponent::set_direction(const glm::vec3 &direction)
{
  directional_light->direction = glm::normalize(direction);
}

} // namespace Fge

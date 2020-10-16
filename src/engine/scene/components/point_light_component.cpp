#include "point_light_component.hpp"
#include "application.hpp"
#include "graphic/point_light.hpp"
#include "scene/component.hpp"
#include "util/assert.hpp"

namespace Fge
{

PointLightComponent::PointLightComponent(Actor *            owner,
                                         int                update_order,
                                         const std::string &type_name)
    : Component(owner, update_order, type_name)
{
}

PointLightComponent::~PointLightComponent()
{
  if (point_light)
  {
    auto app      = Application::get_instance();
    auto renderer = app->get_graphic_manager()->get_renderer();
    renderer->unregister_point_light(point_light);
  }
}

void PointLightComponent::create()
{
  FGE_ASSERT(owner);

  auto app      = Application::get_instance();
  auto renderer = app->get_graphic_manager()->get_renderer();

  point_light           = std::make_shared<PointLight>();
  point_light->position = owner->get_position();

  renderer->register_point_light(point_light);
}

} // namespace Fge

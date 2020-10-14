#include "component.hpp"

namespace Fge
{

Component::Component(Actor *            owner,
                     int                update_order,
                     const std::string &type_name)
    : type_name(type_name),
      owner(owner),
      update_order(update_order)
{
}

void Component::on_create() { create(); }

void Component::on_fixed_update(float frametime) { fixed_update(frametime); }

void Component::on_update(float delta_time) { update(delta_time); }

void Component::on_render() { render(); }

void Component::create() {}

void Component::fixed_update(float /*delta_time*/) {}

void Component::update(float /*delta_time*/) {}

void Component::render() {}

} // namespace Fge

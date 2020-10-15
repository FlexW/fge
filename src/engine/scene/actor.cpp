#include "actor.hpp"
#include "component.hpp"
#include "util/assert.hpp"

namespace Fge
{

Actor::Actor(Scene *scene, std::size_t id, const std::string &name)
    : scene(scene),
      name(name),
      id(id)
{
}

Actor::~Actor() { clear(); }

void Actor::on_create()
{
  compute_world_transform();

  for (auto component : components)
  {
    component->on_create();
  }

  compute_world_transform();
}

void Actor::on_fixed_update(float frametime)
{
  if (state == State::ACTIVE)
  {
    update_components(frametime);
    update_actor_fixed(frametime);

    compute_world_transform();
  }
}

void Actor::on_update(float delta_time)
{
  if (state == State::ACTIVE)
  {
    update_components(delta_time);
    update_actor(delta_time);

    compute_world_transform();
  }
}

void Actor::on_render()
{
  render_components();
  render_actor();
}

void Actor::add_component(std::shared_ptr<Component> component)
{
  FGE_ASSERT(component);

  // Find the insertion point in the sorted vector
  // (The first element with a order higher than me)
  const auto component_order = component->get_update_order();
  auto       components_iter = components.begin();

  for (; components_iter != components.end(); ++components_iter)
  {
    if (component_order < (*components_iter)->get_update_order())
    {
      break;
    }
  }

  // Inserts element before position of iterator
  components.insert(components_iter, component);
}

void Actor::remove_component(std::shared_ptr<Component> component)
{
  auto components_iter =
      std::find(components.begin(), components.end(), component);

  if (components_iter != components.end())
  {
    components.erase(components_iter);
  }
}

void Actor::update_actor_fixed(float /*frametime*/) {}

void Actor::update_actor(float /*delta_time*/) {}

void Actor::render_actor() {}

void Actor::update_components_fixed(float frametime)
{
  const auto size = components.size();
  for (std::size_t i = 0; i < size; ++i)
  {
    compute_world_transform();
    components[i]->on_fixed_update(frametime);
  }
  compute_world_transform();
}

void Actor::update_components(float delta_time)
{
  const auto size = components.size();
  for (std::size_t i = 0; i < size; ++i)
  {
    compute_world_transform();
    components[i]->on_update(delta_time);
  }
  compute_world_transform();
}

void Actor::render_components()
{
  const auto size = components.size();
  for (std::size_t i = 0; i < size; ++i)
  {
    components[i]->on_render();
  }
}

void Actor::update_right() { right = glm::rotate(rotation, RIGHT_VEC); }

void Actor::update_forward() { forward = glm::rotate(rotation, FORWARD_VEC); }

void Actor::update_up() { up = glm::rotate(rotation, UP_VEC); }

void Actor::compute_world_transform()
{
  if (!recompute_world_transform)
  {
    return;
  }

  recompute_world_transform = false;

  world_transform = glm::translate(glm::mat4(1.0f), position);
  world_transform *= glm::toMat4(rotation);
  world_transform = glm::scale(world_transform, scale);
}

void Actor::clear()
{
  while (components.size() != 0)
  {
    remove_component(components[0]);
  }
}

void Actor::set_position(const glm::vec3 &position)
{
  this->position            = position;
  recompute_world_transform = true;
}

void Actor::set_scale(const glm::vec3 &scale)
{
  this->scale               = scale;
  recompute_world_transform = true;
}

void Actor::set_rotation(const glm::quat &rotation)
{
  this->rotation            = rotation;
  rotation_euler            = glm::eulerAngles(this->rotation);
  recompute_world_transform = true;

  update_up();
  update_right();
  update_forward();
}

void Actor::set_rotation_euler(const glm::vec3 &degrees)
{
  rotation_euler            = degrees;
  this->rotation            = glm::quat(degrees);
  recompute_world_transform = true;

  update_up();
  update_right();
  update_forward();
}

} // namespace Fge

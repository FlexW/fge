#pragma once

#include "component.hpp"
#include "math/math.hpp"
#include "std.hpp"

namespace Fge
{

class Scene;

class Actor
{
public:
  enum class State
  {
    ACTIVE,
    PAUSED,
    DEAD
  };

  Actor(Scene *scene, std::size_t id, const std::string &name);

  virtual ~Actor();

  void on_create();

  void on_fixed_update(float frametime);

  void on_update(float delta_time);

  void on_render();

  template <typename T> std::shared_ptr<T> add_component(int update_order)
  {
    auto component = std::make_shared<T>(this, update_order);

    add_component(component);

    return component;
  }

  template <typename T> std::shared_ptr<T> add_component()
  {
    auto component = std::make_shared<T>(this);

    add_component(component);

    return component;
  }

  void add_component(std::shared_ptr<Component> component);

  void remove_component(std::shared_ptr<Component> component);

  std::vector<std::shared_ptr<Component>> get_components()
  {
    return components;
  };

  void set_position(const glm::vec3 &position);

  const glm::vec3 &get_position() const { return position; }

  void set_scale(const glm::vec3 &scale);

  const glm::vec3 &get_scale() const { return scale; }

  void set_rotation_euler(const glm::vec3 &degrees);

  const glm::vec3 &get_rotation_euler() const { return rotation_euler; }

  const glm::quat &get_rotation() const { return rotation; }

  void set_rotation(const glm::quat &rotation);

  Scene *get_scene() const { return scene; }

  void set_state(State state) { this->state = state; }

  State get_state() const { return state; }

  const glm::mat4 &get_world_transform() const { return world_transform; }

  std::size_t get_id() const { return id; }

  const std::string &get_name() const { return name; }

  void set_name(const std::string &name) { this->name = name; }

  const glm::vec3 &get_forward() { return forward; }

  const glm::vec3 &get_right() { return right; }

  std::shared_ptr<Component>
  find_component_by_type_name(const std::string &type_name);

protected:
  Scene *scene{};

  std::string name{};
  std::size_t id{};

  State state = State::ACTIVE;

  glm::vec3 position       = glm::vec3(0.0f);
  glm::vec3 scale          = glm::vec3(1.0f);
  glm::quat rotation       = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3 rotation_euler = glm::vec3(0.0f);

  glm::vec3 up      = UP_VEC;
  glm::vec3 forward = FORWARD_VEC;
  glm::vec3 right   = RIGHT_VEC;

  bool      recompute_world_transform = true;
  glm::mat4 world_transform           = glm::mat4(1.0f);

  std::vector<std::shared_ptr<Component>> components{};

  virtual void update_actor_fixed(float frametime);

  virtual void update_actor(float delta_time);

  virtual void render_actor();

  void update_components_fixed(float frametime);

  void update_components(float delta_time);

  void render_components();

  void update_right();

  void update_forward();

  void update_up();

  void compute_world_transform();

  void clear();
};

} // namespace Fge

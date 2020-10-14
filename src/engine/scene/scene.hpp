#pragma once

#include "actor.hpp"

namespace Fge
{

class Scene
{
public:
  ~Scene();

  void on_create();

  void on_fixed_update(float frametime);

  void on_update(float delta_time);

  void on_render();

  void on_destroy();

  void add_actor(std::shared_ptr<Actor> actor);

  template <typename T>
  std::shared_ptr<T> add_actor(const std::string &name = "Actor")
  {
    auto actor = std::make_shared<T>(this, generate_actor_id(), name);
    add_actor(actor);
    return actor;
  }

  void remove_actor(std::shared_ptr<Actor> actor);

  std::vector<std::shared_ptr<Actor>> &get_actors() { return actors; }

  std::size_t generate_actor_id();

private:
  bool updating_actors = false;

  std::size_t actor_id_count = 0;

  std::vector<std::shared_ptr<Actor>>                     actors;
  std::vector<std::shared_ptr<Actor>>                     pending_actors;
  std::unordered_map<std::size_t, std::shared_ptr<Actor>> ids_to_actors_map;

  void clear();
};

} // namespace Fge

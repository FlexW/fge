#include "scene.hpp"
#include "util/assert.hpp"

namespace Fge
{

Scene::~Scene() { clear(); }

void Scene::on_create()
{
  updating_actors = true;
  for (auto actor : actors)
  {
    actor->on_create();
  }
  updating_actors = false;

  // Move pending actors to actors
  for (auto pending_actor : pending_actors)
  {
    actors.emplace_back(pending_actor);
  }
  pending_actors.clear();

  // Add any dead actors to temp vector
  std::vector<std::shared_ptr<Actor>> dead_actors;
  for (auto actor : actors)
  {
    if (actor->get_state() == Actor::State::DEAD)
    {
      dead_actors.emplace_back(actor);
    }
  }

  // Remove dead actors
  for (auto actor : dead_actors)
  {
    remove_actor(actor);
  }
}

void Scene::on_fixed_update(float frametime)
{
  updating_actors = true;
  for (auto actor : actors)
  {
    actor->on_fixed_update(frametime);
  }
  updating_actors = false;

  // Move pending actors to actors
  for (auto pending_actor : pending_actors)
  {
    actors.emplace_back(pending_actor);
  }
  pending_actors.clear();

  // Add any dead actors to temp vector
  std::vector<std::shared_ptr<Actor>> dead_actors;
  for (auto actor : actors)
  {
    if (actor->get_state() == Actor::State::DEAD)
    {
      dead_actors.emplace_back(actor);
    }
  }

  // Remove dead actors
  for (auto actor : dead_actors)
  {
    remove_actor(actor);
  }
}

void Scene::on_update(float delta_time)
{
  updating_actors = true;
  for (auto actor : actors)
  {
    actor->on_update(delta_time);
  }
  updating_actors = false;

  // Move pending actors to actors
  for (auto pending_actor : pending_actors)
  {
    actors.emplace_back(pending_actor);
  }
  pending_actors.clear();

  // Add any dead actors to temp vector
  std::vector<std::shared_ptr<Actor>> dead_actors;
  for (auto actor : actors)
  {
    if (actor->get_state() == Actor::State::DEAD)
    {
      dead_actors.emplace_back(actor);
    }
  }

  // Remove dead actors
  for (auto actor : dead_actors)
  {
    remove_actor(actor);
  }
}

void Scene::on_render()
{
  for (auto actor : actors)
  {
    actor->on_render();
  }
}

void Scene::on_destroy() { clear(); }

void Scene::add_actor(std::shared_ptr<Actor> actor)
{
  FGE_ASSERT(actor);

  if (ids_to_actors_map.find(actor->get_id()) != ids_to_actors_map.end())
  {
    return;
  }
  ids_to_actors_map[actor->get_id()] = actor;

  if (updating_actors)
  {
    pending_actors.emplace_back(actor);
    return;
  }

  actors.emplace_back(actor);
}

void Scene::remove_actor(std::shared_ptr<Actor> actor)
{
  std::size_t actor_id{};

  // Is it in pending actors?
  auto iter = std::find(pending_actors.begin(), pending_actors.end(), actor);
  if (iter != pending_actors.end())
  {
    actor_id = (*iter)->get_id();
    // Swap to end of vector and pop off (avoid erase copies)
    std::iter_swap(iter, pending_actors.end() - 1);
    pending_actors.pop_back();
    return;
  }

  // Is it in actors?
  iter = std::find(actors.begin(), actors.end(), actor);
  if (iter != actors.end())
  {
    actor_id = (*iter)->get_id();
    // Swap to end of vector and pop off (avoid erase copies)
    std::iter_swap(iter, actors.end() - 1);
    actors.pop_back();
    return;
  }

  ids_to_actors_map.erase(actor_id);
}

std::size_t Scene::generate_actor_id()
{
  ++actor_id_count;

  while (ids_to_actors_map.find(actor_id_count) != ids_to_actors_map.end())
  {
    ++actor_id_count;
  }

  return actor_id_count;
}

void Scene::clear()
{
  while (pending_actors.size() != 0)
  {
    remove_actor(actors[0]);
  }

  while (actors.size() != 0)
  {
    remove_actor(actors[0]);
  }
}

} // namespace Fge

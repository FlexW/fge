#pragma once

#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "ecs/type_info.hpp"
#include "entity.hpp"
#include "sparse_set.hpp"
#include "storage.hpp"
#include "util/assert.hpp"
#include "view.hpp"

namespace fge::ecs
{

/**
 * @brief Entity-Component System
 *
 * The entity component system is modeled after the famous library EnTT.
 *
 * The registry is the manager of the entity component system. It's
 * the place where entities and components get added and removed.
 *
 * @tparam TEntity Entity type
 */
template <typename TEntity> class basic_registry
{
public:
  /**
   * @brief Create a new entity.
   *
   * @return A new entity.
   */
  TEntity create()
  {
    return entites_to_recycle.size() == 0 ? generate_entity()
                                          : recycle_entity();
  }

  /**
   * @brief Remove a entity.
   *
   * @param entity Entity to remove.
   */
  void destroy(const TEntity &entity)
  {
    const auto entity_id = static_cast<typename TEntity::id_type>(entity.id());
    FGE_ASSERT(entity_id < entities.size());

    entities[entity_id] = null;
    entites_to_recycle.emplace_back(entity);
  }

  /**
   * Checks if the entity is valid.
   *
   * A entity might be invalid if it was removed from the registry.
   *
   * @param entity Entity to check.
   *
   * @return Whether the entity is valid or not.
   */
  bool valid(const TEntity &entity) const
  {
    if (entity.id() >= entities.size())
    {
      return false;
    }
    else if (entities[entity.id()] == null)
    {
      return false;
    }
    return true;
  }

  /**
   * @brief Add a component to a entity.
   *
   * @param entity Entity where component should be added.
   * @param component_args Arguments for component.
   *
   * @return The created component.
   */
  template <typename TComponent, typename... TArgs>
  decltype(auto) emplace(TEntity entity, TArgs &&...component_args)
  {
    auto component_pool = assure<TComponent>();
    return component_pool->emplace(entity,
                                   std::forward<TArgs>(component_args)...);
  };

  /**
   * @brief Return the component of the given entity.
   *
   * A invalid entity or if the component does not exist on the
   * entity, will result in undefined behavior.
   *
   * @param entity Entity
   *
   * @return Component
   */
  template <typename TComponent> decltype(auto) get(const TEntity &entity)
  {
    auto component_pool = assure<TComponent>();
    return component_pool->get(entity);
  }

  /**
   * @brief Return the component of the given entity if possible.
   *
   * A invalid entity will result in undefined behavior.
   *
   * @param entity Entity
   *
   * @return Component
   */
  template <typename TComponent>
  std::optional<TComponent *> try_get(const TEntity entity)
  {
    auto component_pool = assure<TComponent>();
    return component_pool->try_get(entity);
  }

  /**
   * Removes the component from the given entity
   *
   * A invalid entity will result in undefined behavior.
   *
   * @tparam TComponent Component
   * @param entity Entity
   */
  template <typename TComponent> void remove(const TEntity entity)
  {
    auto component_pool = assure<TComponent>();
    component_pool->remove(entity);
  }

  template <typename... TComponent> basic_view<TEntity> view()
  {
    return {assure<TComponent>()...};
  }

private:
  std::vector<TEntity> entities;
  std::vector<TEntity> entites_to_recycle;

  std::vector<std::unique_ptr<basic_sparse_set<TEntity>>> component_pools;

  TEntity generate_entity()
  {
    // TEntity::max_id() is reserved to allow for null identifiers
    const auto new_id = static_cast<typename TEntity::id_type>(entities.size());
    FGE_ASSERT(new_id < TEntity::max_id());

    return entities.emplace_back(new_id);
  }

  TEntity recycle_entity()
  {
    FGE_ASSERT(entites_to_recycle.size() > 0);

    auto entity_to_recycle = entites_to_recycle.back();
    entites_to_recycle.pop_back();

    entity_to_recycle.increment_version();
    entities[entity_to_recycle.id()] = entity_to_recycle;

    return entity_to_recycle;
  }

  template <typename TComponent> basic_storage<TEntity, TComponent> *assure()
  {
    const auto component_type_id = sequential_type_info::type<TComponent>();
    if (component_type_id >= component_pools.size())
    {
      component_pools.resize(component_type_id + 1);
    }

    if (!component_pools[component_type_id])
    {
      component_pools[component_type_id] =
          std::make_unique<basic_storage<TEntity, TComponent>>();
    }

    auto component_pool = static_cast<basic_storage<TEntity, TComponent> *>(
        component_pools[component_type_id].get());

    return component_pool;
  }
};

} // namespace fge::ecs

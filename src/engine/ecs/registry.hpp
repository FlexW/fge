#pragma once

#include <vector>

#include "entity.hpp"
#include "util/assert.hpp"

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
  void remove(TEntity entity)
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
  bool valid(TEntity entity)
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

private:
  std::vector<TEntity> entities;
  std::vector<TEntity> entites_to_recycle;

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
};

} // namespace fge::ecs

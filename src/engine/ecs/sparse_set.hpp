#pragma once

#include <vector>

#include "util/assert.hpp"

namespace fge::ecs
{

/**
 * @brief Sparse set implementation for entities.
 *
 * @tparam TEntity Type of entity.
 */
template <typename TEntity> class basic_sparse_set
{
public:
  void emplace(TEntity entity)
  {
    FGE_ASSERT(entity.id() >= entity_indices.size());

    entity_indices.resize(entity.id() + 1);

    const auto entity_index = entities.size();
    entities.emplace_back(entity);
    entity_indices[entity.id()] = entity_index;
  }

  typename TEntity::id_type index(TEntity entity)
  {
    FGE_ASSERT(entity.id() < entity_indices.size());

    return entity_indices[entity.id()];
  }

private:
  std::vector<typename TEntity::id_type> entity_indices;
  std::vector<TEntity>                   entities;
};
}

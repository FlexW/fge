#pragma once

#include <bits/c++config.h>
#include <vector>

#include "entity.hpp"
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
    FGE_ASSERT(entity.id() >= sparse_set.size());

    sparse_set.resize(entity.id() + 1);

    const auto entity_index = dense_set.size();

    dense_entry entity_dense_entry;
    entity_dense_entry.entity       = entity;
    entity_dense_entry.sparse_index = entity.id();
    dense_set.emplace_back(entity_dense_entry);

    sparse_entry entity_sparse_entry;
    entity_sparse_entry.dense_index = entity_index;
    sparse_set[entity.id()]         = entity_sparse_entry;
  }

  typename TEntity::id_type index(TEntity entity) const
  {
    FGE_ASSERT(entity.id() < sparse_set.size());

    return sparse_set[entity.id()].dense_index;
  }

  bool contains(const TEntity entity) const
  {
    if (entity.id() >= sparse_set.size())
    {
      return false;
    }

    if (!sparse_set[entity.id()].valid)
    {
      return false;
    }

    return true;
  }

  void remove(const TEntity entity)
  {
    FGE_ASSERT(entity.id() < sparse_set.size());

    const auto dense_entry_to_swap = dense_set.back();

    swap_and_pop(sparse_set[entity.id()].dense_index);

    dense_set[sparse_set[entity.id()].dense_index] = dense_entry_to_swap;
    sparse_set[dense_entry_to_swap.sparse_index].dense_index =
        sparse_set[entity.id()].dense_index;

    sparse_set[entity.id()].valid = false;
    dense_set.pop_back();
  }

protected:
  virtual void swap_and_pop(typename TEntity::id_type new_index) = 0;

private:
  struct sparse_entry
  {
    typename TEntity::id_type dense_index;
    bool                      valid = true;
  };

  struct dense_entry
  {
    typename TEntity::id_type sparse_index;
    TEntity     entity;
  };

  std::vector<sparse_entry> sparse_set;
  std::vector<dense_entry>  dense_set;
};
} // namespace fge::ecs

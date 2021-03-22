#pragma once

#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

#include "sparse_set.hpp"
#include "util/assert.hpp"

namespace fge::ecs
{

/**
 * @brief Stores components for a entity.
 *
 * Extends sparse sets for storage of components.
 *
 * @tparam TEntity Type of entity.
 * @tparam TComponent Type of component.
 */
template <typename TEntity, typename TComponent>
class basic_storage : public basic_sparse_set<TEntity>
{
public:
  template <typename... TArgs>
  decltype(auto) emplace(TEntity entity, TArgs &&...component_args)
  {
    // TODO: Assumes same layout like in sparse set -> Tight coupeling

    if constexpr (std::is_aggregate_v<TComponent>)
    {
      components.push_back(TComponent{std::forward<TArgs>(component_args)...});
    }
    else
    {
      components.emplace_back(std::forward<TArgs>(component_args)...);
    }

    underlying_type::emplace(entity);

    return components.back();
  }

  decltype(auto) get(TEntity entity)
  {
    const auto component_index = underlying_type::index(entity);

    FGE_ASSERT(component_index < components.size());

    return components[component_index];
  }

  std::optional<TComponent *> try_get(const TEntity entity)
  {
    if (!underlying_type::contains(entity))
    {
      return {};
    }

    return &get(entity);
  }

  void remove(const TEntity entity)
  {
    underlying_type::remove(entity);
    const auto component_index = underlying_type::index(entity);
  }

protected:
  void swap_and_pop(typename TEntity::id_type new_index) override
  {
    const auto component = components.back();
    components[new_index] = component;
    components.pop_back();
  }

private:
  using underlying_type = basic_sparse_set<TEntity>;

  std::vector<TComponent> components;
};

} // namespace fge::ecs

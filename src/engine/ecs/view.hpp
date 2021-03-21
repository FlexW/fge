#pragma once

#include <utility>
#include <vector>

#include "ecs/sparse_set.hpp"
#include "ecs/storage.hpp"

namespace fge::ecs
{

/**
 * @brief View into the registry
 *
 * Views should never be stored in a variable.
 */
template <typename TEntity> class basic_view
{
public:
  template <typename... TComponent>
  basic_view(const basic_storage<TEntity, TComponent> *...components)
      : view(smallest_storage(components...))
  {
  }

  decltype(auto) entities() const { return view->entities(); }

private:
  const basic_sparse_set<TEntity> *view;

  template <typename... TComponent>
  auto smallest_storage(const basic_storage<TEntity, TComponent> *...components)
  {
    return std::min(
        {static_cast<const basic_sparse_set<TEntity> *>(components)...},
        [](const auto *lhs, const auto *rhs) {
          return lhs->size() < rhs->size();
        });
  }
};

} // namespace fge::ecs

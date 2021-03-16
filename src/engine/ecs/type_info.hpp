#pragma once

#include <bits/c++config.h>

namespace fge::ecs
{

/**
 * @brief Generator for sequential type ids
 *
 * For two different types this generator will generate two unique
 * ids. The ids will be in sequential order.
 */
class sequential_type_info
{
public:
  /**
   * @brief Get id of given type
   */
  template <typename> static std::size_t type() noexcept
  {
    static const std::size_t value = id();
    return value;
  }

private:
  static std::size_t id() noexcept
  {
    static std::size_t value = 0;
    return value++;
  }
};
};

#pragma once

#include <cassert>
#include <type_traits>

namespace fge::gfx
{

template <typename TEnum> struct enable_bit_mask_operators
{
  static const bool enable = false;
};

template <typename TEnum>
constexpr typename std::enable_if<enable_bit_mask_operators<TEnum>::enable,
                                  TEnum>::type
operator|(TEnum x, TEnum y)
{
  using underlying_type = typename std::underlying_type<TEnum>::type;
  return static_cast<TEnum>(static_cast<underlying_type>(x) |
                            static_cast<underlying_type>(y));
}

template <typename TEnum>
constexpr typename std::enable_if<enable_bit_mask_operators<TEnum>::enable,
                                  TEnum>::type
operator&(TEnum x, TEnum y)
{
  using underlying_type = typename std::underlying_type<TEnum>::type;
  return static_cast<TEnum>(static_cast<underlying_type>(x) &
                            static_cast<underlying_type>(y));
}

#define FGE_ENABLE_BITMASK_OPERATORS(x)                                        \
  template <> struct enable_bit_mask_operators<x>                              \
  {                                                                            \
    static const bool enable = true;                                           \
  };

#define FGE_ASSERT(x) assert(x)

#define FGE_FAIL(msg) assert(0 && msg)

} // namespace fge::gfx

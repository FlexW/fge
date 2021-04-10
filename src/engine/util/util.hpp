#pragma once

#include <type_traits>

template <typename TEnum> struct enable_bit_mask_operators
{
  static const bool enable = false;
};

namespace fge {

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

} // namespace fge

#define FGE_ENABLE_BITMASK_OPERATORS(x)                                        \
  template <> struct enable_bit_mask_operators<x>                              \
  {                                                                            \
    static const bool enable = true;                                           \
  };

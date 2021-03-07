#pragma once

#include <cstddef>
#include <cstdint>

#include "util.hpp"

namespace fge
{

constexpr std::size_t align_to(std::size_t value, std::size_t alignment)
{
  return static_cast<std::size_t>(
      (static_cast<std::size_t>(value) + (alignment - 1)) & ~(alignment - 1));
}

template <typename TSize>
constexpr bool can_satisfy_allocation(uintptr_t   buffer,
                                      TSize       buffer_size,
                                      TSize       allocated_size,
                                      std::size_t size,
                                      std::size_t alignment)
{
  const uintptr_t buffer_head = buffer + allocated_size;
  const uintptr_t alloc_start = align_to(buffer_head, alignment);

  if (alloc_start < buffer_head)
  {
    // Alignment does overflow
    return false;
  }

  const uintptr_t alloc_end = alloc_start + size;
  if (alloc_end <= alloc_start)
  {
    // Size does overflow
    return false;
  }

  const uintptr_t alloc_size         = alloc_end - buffer_head;
  const TSize     new_allocated_size = allocated_size + alloc_size;

  if (new_allocated_size <= buffer_size)
  {
    // Enough space
    return true;
  }

  // Not enough space
  return false;
}

template <typename TSize> struct AllocResult
{
  void *data;
  TSize allocated_size;
};

template <typename TSize>
AllocResult<TSize> allocate_from_buffer(uintptr_t   buffer,
                                        TSize       allocated_size,
                                        std::size_t size,
                                        std::size_t alignment)
{
  const uintptr_t buffer_head        = buffer + allocated_size;
  const uintptr_t alloc_start        = align_to(buffer_head, alignment);
  const uintptr_t alloc_end          = alloc_start + size;
  const uintptr_t alloc_size         = alloc_end - buffer_head;
  const TSize     new_allocated_size = allocated_size + alloc_size;

  AllocResult<TSize> alloc_result{};
  alloc_result.data           = reinterpret_cast<void *>(alloc_start);
  alloc_result.allocated_size = new_allocated_size;

  return alloc_result;
}

} // namespace Fge

#pragma once

#include "allocator.hpp"
#include "util.hpp"

namespace fge
{

template <typename TSize> class LinearAllocator
{
public:
  LinearAllocator(void *buffer, TSize buffer_size)
      : buffer(reinterpret_cast<uintptr_t>(buffer)),
        buffer_size(buffer_size)
  {
    init();
  }

  LinearAllocator() : buffer(0), buffer_size(0) {}

  LinearAllocator(LinearAllocator<TSize> &&allocator)
  {
    buffer         = allocator.buffer;
    buffer_size    = allocator.buffer_size;
    allocated_size = allocator.allocated_size;

    allocator.buffer         = 0;
    allocator.buffer_size    = 0;
    allocator.allocated_size = 0;
  }

  void operator=(LinearAllocator<TSize> &&allocator)
  {
    buffer         = allocator.buffer;
    buffer_size    = allocator.buffer_size;
    allocated_size = allocator.allocated_size;

    allocator.buffer         = 0;
    allocator.buffer_size    = 0;
    allocator.allocated_size = 0;
  }

  void *allocate(TSize size, std::size_t alignment)
  {
    FGE_ASSERT(alignment % 2 == 0);

    if (!can_satisfy_allocation(buffer,
                                buffer_size,
                                allocated_size,
                                size,
                                alignment))
    {
      FGE_FAIL("Can not allocate more memory");
    }

    const auto alloc_result =
        allocate_from_buffer(buffer, allocated_size, size, alignment);

    allocated_size = alloc_result.allocated_size;

    return alloc_result.data;
  }

  void reset() { allocated_size = 0; }

private:
  uintptr_t   buffer{};
  TSize       buffer_size{};
  TSize       allocated_size{};

  void init()
  {
    FGE_ASSERT(buffer != 0);
    FGE_ASSERT(buffer_size != 0);

    allocated_size = 0;
  }
};

using linear_allocator = LinearAllocator<size_t>;

} // namespace Fge

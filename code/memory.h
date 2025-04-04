#ifndef memory_h_already_included
#define memory_h_already_included

#include "common_data_types.h"

#define Kilobytes(n) ((n) * 1024)
#define Megabytes(n) (Kilobytes(n) * 1024)

// An "arena allocator". TODO: Explain!
struct MemoryAllocator
{
  // The underlying block of memory controlled by this allocator.
  Memory memory;

  // How many bytes in total does this allocator control?
  Size memory_s;

  // How many bytes have been allocated?
  Size allocated_s;
};

struct MemoryAllocator MemoryAllocator(
  Memory memory,
  Size memory_s);

Memory NextAddressToAllocate(const struct MemoryAllocator* allocator);

Memory Allocate(
  struct MemoryAllocator*
  allocator,
  Size allocation_s);

Memory AllocateCopy(
  struct MemoryAllocator* allocator,
  Memory memory_to_copy_from,
  Size copy_s);

void ResetAllocator(struct MemoryAllocator* allocator);

#endif

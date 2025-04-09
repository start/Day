#ifndef memory_h_already_included
#define memory_h_already_included

#include "common_data_types.h"


// An "arena allocator". TODO: Explain!
struct Allocator
{
  // The underlying block of memory controlled by this allocator.
  Memory memory;

  // How many bytes in total does this allocator control?
  Size memory_s;

  // How many bytes have been allocated?
  Size allocated_s;
};

struct Allocator Allocator(
  Memory memory,
  Size memory_s);

Memory NextAddressToAllocate(const struct Allocator* allocator);

Memory Allocate(
  struct Allocator*
  allocator,
  Size allocation_s);

Memory AllocateCopy(
  struct Allocator* allocator,
  Memory copy_from,
  Size copy_s);

void ResetAllocator(struct Allocator* allocator);

#endif

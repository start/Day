#ifndef memory_h_already_included
#define memory_h_already_included

#include "basic_types.h"

#define Kilobytes(n) ((n) * 1024)
#define Megabytes(n) (Kilobytes(n) * 1024)

/*
  In C, we determine the number of items in an array by dividing
  its [total size in bytes] by its [first item's size in bytes].
*/
#define ArraySize(array) (sizeof array / sizeof array[0])

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

struct MemoryAllocator CreateMemoryAllocator(Memory memory, Size memory_s);
Memory NextAddressToAllocate(const struct MemoryAllocator* allocator);
void ResetMemoryAllocator(struct MemoryAllocator* allocator);
Memory AllocateMemory(struct MemoryAllocator* allocator, Size needed_s);

#endif

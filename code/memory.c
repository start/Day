#include <stdlib.h>
#include "memory.h"
#include "basic_types.h"

// Create a new memory allocator.
struct MemoryAllocator CreateMemoryAllocator(
  Byte* memory,
  const Size memory_s)
{
  return (struct MemoryAllocator)
  {
    .memory = memory,
    .memory_s = memory_s
  };
}

/*
  Returns a pointer to the newly allocated memory.

  If the provided allocator doesn't have enough memory to perform
  the allocation, we immediately terminate the program.
*/
Memory AllocateMemory(
  struct MemoryAllocator* allocator,
  Size needed_s)
{
  // Do we need more bytes than this allocator has available?
  if (needed_s > (allocator->memory_s - allocator->allocated_s))
  {
    // This allocator wasn't given enough memory. Let's exit our
    // program then go fix the bug.
    exit(EXIT_FAILURE);
  }

  // Record the memory address we're allocating into.
  const auto allocation_address =
    allocator->memory + allocator->allocated_s;

  // Record the number of bytes allocated.
  allocator->allocated_s += needed_s;

  return allocation_address;
}

// Free all allocated memory.
void ResetMemoryAllocator(struct MemoryAllocator* allocator)
{
  allocator->allocated_s = 0;
}
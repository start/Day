#include <stdlib.h>
#include "memory.h"
#include "basic_types.h"

// Create a new memory allocator.
struct MemoryAllocator CreateMemoryAllocator(
  Memory memory,
  const Size memory_s)
{
  return (struct MemoryAllocator)
  {
    .memory = memory,
    .memory_s = memory_s
  };
}

// What's the address of the next memory the given allocator will
// allocate?
Memory NextAddressToAllocate(const struct MemoryAllocator* allocator)
{
  /*
    In C, when we add 1 to a pointer's value, we're not actually
    adding 1! Instead, we're adding the [size in bytes] of the
    pointer's underlying data type. If that datatype is 4 bytes
    wide, then we're actually adding 4. Make sense?

    Our 'Memory' data type points to the generic 'void' type. By
    rule, 'void' doesn't have any defined size; it's deliberately
    vague. C requires us to "cast", or convert, void pointers to
    a more specific data type before involving them in arithmetic
    operations.

    Since we want our allocator to be able to allocate individual
    bytes, we cast our memory pointer to 'Byte*' before we add
    to it the number of bytes we've already allocated.
  */
  return (Byte*) allocator->memory + allocator->allocated_s;
}

// Free all allocated memory.
void ResetMemoryAllocator(struct MemoryAllocator* allocator)
{
  allocator->allocated_s = 0;
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
  const auto allocation_address = NextAddressToAllocate(allocator);

  // Record the number of bytes allocated.
  allocator->allocated_s += needed_s;

  return allocation_address;
}

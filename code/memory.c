#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "common_data_types.h"

// Returns a new memory allocator, provided a region of memory
// for the allocator to control.
struct Allocator Allocator(
  Memory memory_to_allocate_from,
  const Size memory_s)
{
  return (struct Allocator)
  {
    .memory = memory_to_allocate_from,
    .memory_s = memory_s
  };
}


/*
  The next time we allocate memory, which address will our
  allocator provide?
*/
Memory NextAddressToAllocate(const struct Allocator* allocator)
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


/*
  Returns a pointer to the newly allocated memory.

  If the provided allocator doesn't have enough memory to perform
  the allocation, we immediately terminate the program.
*/
Memory Allocate(
  struct Allocator* allocator,
  // How many bytes do we need to allocate?
  Size allocation_s)
{
  // Do we need more bytes than this allocator has available?
  if (allocation_s > (allocator->memory_s - allocator->allocated_s))
  {
    // This allocator wasn't given enough memory. Let's exit our
    // program then go fix the bug.
    exit(EXIT_FAILURE);
  }

  // Record the memory address we're allocating into.
  const auto allocation = NextAddressToAllocate(allocator);

  // Record the number of bytes allocated.
  allocator->allocated_s += allocation_s;

  return allocation;
}


// Copies into the given allocator.
Memory AllocateCopy(
  // Copy into this allocator.
  struct Allocator* allocator,
  // Where are we copying from?
  Memory copy_from,
  // How many bytes are we copying?
  Size copy_s)
{
  auto copy_to = Allocate(allocator, copy_s);
  return memcpy(copy_to, copy_from, copy_s);
}


// Performs a factory reset on the given allocator.
void ResetAllocator(struct Allocator* allocator)
{
  allocator->allocated_s = 0;
}
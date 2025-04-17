#include "memory.h"
#include "common_data_types.h"
#include "exit_with_error.h"
#include <stdlib.h>
#include <string.h>


// Returns a new memory allocator, provided a region of memory
// for the allocator to control.
struct Allocator Allocator(
  Memory memory_to_allocate_from,
  Size memory_w)
{
  return (struct Allocator)
  {
    .memory = memory_to_allocate_from,
    .memory_w = memory_w
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
  return (Byte*) allocator->memory + allocator->allocated_w;
}


/*
  Returns a pointer to the newly allocated memory.

  If the provided allocator doesn't have enough memory to perform
  the allocation, we immediately terminate the program.
*/
Memory Allocate(
  struct Allocator* allocator,
  // How many bytes do we need to allocate?
  Size allocation_w)
{
  // Do we need more bytes than this allocator has available?
  if (allocation_w > (allocator->memory_w - allocator->allocated_w))
  {
    // This allocator wasn't given enough memory. Let's exit our
    // program then go fix the bug.
    ExitWithError(
      "Allocator ran out of memory!\n"
      "  Requested: %zu bytes\n"
      "  Total: %zu bytes\n",
      "  Used: %zu bytes\n",
      allocation_w,
      allocator->memory_w,
      allocator->allocated_w);
  }

  // Record the memory address we're allocating into.
  auto allocation = NextAddressToAllocate(allocator);

  // Record the number of bytes allocated.
  allocator->allocated_w += allocation_w;

  return allocation;
}


// Copies into the given allocator.
Memory AllocateCopy(
  // Copy into this allocator.
  struct Allocator* allocator,
  // Where are we copying from?
  Memory copy_from,
  // How many bytes are we copying?
  Size copy_w)
{
  auto copy_to = Allocate(allocator, copy_w);
  return memcpy(copy_to, copy_from, copy_w);
}


// Performs a factory reset on the given allocator.
void ResetAllocator(struct Allocator* allocator)
{
  allocator->allocated_w = 0;
}
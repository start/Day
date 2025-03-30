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
  Byte* memory;
  Size memory_s;
  Size allocated_s;
};

struct MemoryAllocator CreateMemoryAllocator(Byte* memory, Size memory_s);
Memory AllocateMemory(struct MemoryAllocator* allocator, Size needed_s);
void ResetMemoryAllocator(struct MemoryAllocator* allocator);



#endif

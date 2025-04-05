#include <stdio.h>
#include <string.h>
#include "code/common_data_types.h"
#include "code/chunking.h"
#include "code/memory.h"


/*
  When our program runs, C calls this function. It's where our
  program begins.

  C is very strict about this function! It must be named "main"
  with all lowercase letters, it must return an integer, and its
  arguments must have specific types.
*/
Integer main(Integer argument_count, Text arguments[])
{
  constexpr auto how_much_memory_this_uses = Megabytes(4);
  Byte memory[how_much_memory_this_uses];
  auto allocator = MemoryAllocator(memory, how_much_memory_this_uses);

  const auto line = u8"  give vector x";

  const auto chunked_line_of_code =
    ChunkedLineOfCode(line, &allocator);

  printf("Indent level: %f\n", chunked_line_of_code.indent_level);
  printf("Chunk count: %lu\n", chunked_line_of_code.code_chunks_s);

  for (auto i = 0; i < chunked_line_of_code.code_chunks_s; i++)
  {
    printf("chunked_line_of_code[%i]: %s\n", i, chunked_line_of_code.code_chunks[i]);
  }

  // By returning 0, we tell C, "We finished successfully!"
  return 0;
}

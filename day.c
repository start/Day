#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code/common_data_types.h"
#include "code/straining.h"
#include "code/memory.h"


void Render(struct StrainedLineOfCode strained);

/*
  When our program runs, C calls this function. It's where our
  program begins.

  C is very strict about this function! It must be named "main"
  with all lowercase letters, it must return an integer, and its
  arguments must have specific types.
*/
Integer main(Integer argument_count, Text arguments[])
{
  // The first argument is always the name of the program. Any
  // user-specified arguments follow.
  if (argument_count == 1)
  {
    printf("You didn't specify a Day source file.");
    exit(EXIT_FAILURE);
  }

  FILE *day_source_file; {
    auto filename = arguments[1];
    day_source_file = fopen(filename, "r");

    if (day_source_file == NULL)
    {
      printf("Day couldn't open your source file: %s!\n", filename);
      exit(EXIT_FAILURE);
    }

    Byte straining_memory[needed_memory_for_straining_a_line];
    auto allocator = Allocator(
      straining_memory,
      sizeof straining_memory);

    Character line_buffer[max_line_length];
    while (fgets(line_buffer, sizeof line_buffer, day_source_file))
    {
      Render(StrainedLineOfCode(line_buffer, &allocator));
      ResetAllocator(&allocator);
    }
  } fclose(day_source_file);

  // By returning 0, we tell C, "We didn't fail!"
  return 0;
}


void Render(struct StrainedLineOfCode strained)
{
  printf("Indent level: %f\n", strained.indent_level);
  printf("Chunk count: %lu\n", strained.code_chunks_s);

  for (auto i = 0; i < strained.code_chunks_s; i++)
  {
    printf("  '%s'\n", strained.code_chunks[i]);
  }
}

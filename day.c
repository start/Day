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
  FILE *day_source_file;
  {
    const auto filename = "new.day";
    day_source_file = fopen(filename, "r");

    if (day_source_file == NULL)
    {
      printf("Day couldn't open your source file: %s!\n");
      exit(EXIT_FAILURE);
    }

    Byte allocator_memory[Kilobytes(2)];
    auto allocator =
      Allocator(allocator_memory, sizeof allocator_memory);

    Character line_buffer[max_line_length];
    while (fgets(line_buffer, sizeof(line_buffer), day_source_file))
    {
      Render(StrainedLineOfCode(line_buffer, &allocator));
    }
  }
  fclose(day_source_file);

  // By returning 0, we tell C, "We finished successfully!"
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
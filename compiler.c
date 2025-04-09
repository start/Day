#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code/common_data_types.h"
#include "code/tokenizing.h"
#include "code/memory.h"


void Render(struct TokenizedLine tokenized);

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
  // user-specified arguments follow it.
  if (argument_count == 1)
  {
    printf("You didn't specify a T source file.");
    exit(EXIT_FAILURE);
  }

  FILE *day_source_file; {
    auto filename = arguments[1];
    day_source_file = fopen(filename, "r");

    if (day_source_file == NULL)
    {
      printf("The compiler couldn't read your source file: %s!\n", filename);
      exit(EXIT_FAILURE);
    }

    Byte straining_memory[needed_memory_for_straining_a_line];
    auto allocator = Allocator(
      straining_memory,
      sizeof straining_memory);

    Character line_buffer[max_line_length];
    while (fgets(line_buffer, sizeof line_buffer, day_source_file))
    {
      Render(TokenizedLine(line_buffer, &allocator));
      ResetAllocator(&allocator);
    }
  } fclose(day_source_file);

  // By returning 0, we tell C, "We didn't fail!"
  return 0;
}


void Render(struct TokenizedLine tokenized)
{
  printf("Indent level: %f\n", tokenized.indent_level);
  printf("Token count: %lu\n", tokenized.tokens_s);

  for (auto i = 0; i < tokenized.tokens_s; i++)
  {
    printf("  '%s'\n", tokenized.tokens[i]);
  }
}

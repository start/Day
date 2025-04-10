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
    fprintf(stderr, "You need to specify a T source file.\n");
    exit(EXIT_FAILURE);
  }

  FILE *t_source_file; {
    auto filename = arguments[1];
    t_source_file = fopen(filename, "r");

    if (t_source_file == NULL)
    {
      // [this message]: [underlying 'errno' message]
      perror("The compiler couldn’t open your source file");
      fprintf(stderr, "Provided filename: '%s'\n", filename);

      exit(EXIT_FAILURE);
    }

    // TODO: Explain 2.
    Character line_buffer[(max_line_length * utf8_max_character_width) + 2];
    while (fgets(line_buffer, sizeof line_buffer, t_source_file))
    {
      Byte tokenizing_memory[needed_memory_for_tokenizing_a_line];
      auto tokenizing_allocator =
        Allocator(tokenizing_memory, sizeof tokenizing_memory);

      Render(TokenizedLine(line_buffer, &tokenizing_allocator));
    }
  } fclose(t_source_file);

  // By returning 0, we tell C, "We didn't fail!"
  return 0;
}


void Render(struct TokenizedLine tokenized)
{
  printf("Indent level: %f\n", tokenized.indent_level);
  printf("Token count: %lu\n", tokenized.tokens_w);

  for (auto i = 0; i < tokenized.tokens_w; i++)
  {
    printf("  '%s'\n", tokenized.tokens[i]);
  }
}

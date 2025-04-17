#include "code/common_data_types.h"
#include "code/exit_with_error.h"
#include "code/tokenizing.h"
#include "code/memory.h"
#include <stdio.h>


void Render(const struct TokenizedLine *tokenized);

Integer main(Integer argument_count, Text arguments[])
{
  // The first argument is always the name of the program. Any
  // user-specified arguments follow it.
  if (argument_count == 1)
  {
    ExitWithError("You need to specify a T source file.\n");
  }

  FILE *t_source_file; {
    auto filename = arguments[1];
    t_source_file = fopen(filename, "r");

    if (t_source_file == NULL)
    {
      // [this message]: [underlying 'errno' message]
      perror("The compiler couldn’t open your source file");
      ExitWithError("Provided filename: '%s'\n", filename);
    }

    // In bytes, what's the widest a line can be?
    constexpr Size widest_line_w =
      max_line_length * utf8_max_character_width;

    // We add 2 to accommodate a trailing '\n' and '\0'.
    Character line_buffer[2 + widest_line_w];
    /*
      The 'fgets' function always appends '\0' to the line
      buffer, so we always need room for that byte.

      'fgets' will normally include a '\n', too, but the '\n'
      will be missing in two situations:

      1. The final line of the file doesn't end with a '\n'.
      2. The line is longer than our buffer. In this case,
         'fgets' will truncate the line.

      We always make room for both the '\n' and '\0'. If 'fgets'
      fills our line buffer completely without including a '\n',
      we know the line is too long--it must have been truncated.
    */

    // Which line are we on?
    Size line_number = 1;

    while (fgets(line_buffer, sizeof line_buffer, t_source_file))
    {
      Byte tokenizing_memory[bytes_needed_to_tokenize_a_line];
      auto tokenizing_allocator =
        Allocator(tokenizing_memory, sizeof tokenizing_memory);

      auto tokenized_line =
        TokenizedLine(line_buffer, line_number, &tokenizing_allocator);

      Render(&tokenized_line);

      line_number += 1;
    }
  } fclose(t_source_file);

  return 0;
}


void Render(const struct TokenizedLine *tokenized)
{
  printf("Indent level: %zu\n", tokenized->indent_level);
  printf("Token count: %zu\n", tokenized->tokens_w);

  for (Offset i = 0; i < tokenized->tokens_w; i++)
  {
    printf("  '%s'\n", tokenized->tokens[i]);
  }
}

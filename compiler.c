#include "code/common_data_types.h"
#include "code/exit_due_to_error.h"
#include "code/tokenizing.h"
#include "code/memory.h"
#include <stdio.h>


// C requires us to announce a function's definition before we’re
// allowed to use the function.
void Render(const struct TokenizedLine *tokenized, Offset line_number);


// Our program starts here.
Integer main(Integer argument_count, Text arguments[])
{
  // The first argument is always the name of the program. Any
  // user-specified arguments follow it.
  if (argument_count == 1)
  {
    ExitDueToError("You need to specify a T source file.\n");
  }

  // For now, we only accept a single user-specified argument:
  // the T source file.
  if (argument_count > 2)
  {
    ExitDueToError("You provided too many arguments.\n");
  }

  /*
    Q: Why are we introducing a new scope, demarcated by { ... }?

    A: This little construct helps us to remember to close the
       file once we're done with it, which is right after the
       matching closing curly brace.
  */
  FILE *t_source_file; {
    auto filename = arguments[1];
    t_source_file = fopen(filename, "r");

    // Did we manage to open the file?
    if (t_source_file == NULL)
    {
      // Nope.
      ExitDueToError(
        "The compiler couldn’t open your source file: '%s'\n",
        filename);
    }

    constexpr Size max_bytes_per_line =
      max_line_length * utf8_max_character_width;

    // We add 2 to accommodate a trailing '\n' and '\0'.
    Character line_buffer[2 + max_bytes_per_line];
    /*
      The 'fgets' function always appends '\0' to the line, so we
      always need room for that byte.

      Usually, the line buffer includes the trailing '\n', too,
      but this character is missing in two situations:

      1. If final line of the source file doesn't end with a
         newline character, 'fgets' won't append one.

      2. If any given line is longer than our buffer, 'fgets'
         truncates the line to make sure it fits. This chops off
         the newline character.

      We always make extra room for both the '\n' and '\0'. If
      'fgets' completely fills this "expanded" buffer without
      including a newline character, the line was longer than we
      allow.
    */

    // Which line are we currently tokenizing?
    Offset line_number = 1;

    while (fgets(line_buffer, sizeof line_buffer, t_source_file))
    {
      // Create our allocator using blazing-fast stack memory.
      Byte allocator_memory[bytes_needed_to_tokenize_a_line];
      /*
        Q: Why do we create a new allocator each loop iteration?

        A: Right now, we don't need to preserve any data between
           iterations. Once we do, we'll need to use an allocator
           that is preserved between iterations, too.
      */
      auto allocator =
        Allocator(allocator_memory, sizeof allocator_memory);

      // Tokenize the current line.
      auto tokenized_line =
        TokenizedLine(line_buffer, line_number, &allocator);

      // Render the result!
      Render(&tokenized_line, line_number);

      // Ever onward.
      line_number += 1;
    }
  } fclose(t_source_file);

  // Could we pretend that this 0 m
  return 0;
}


// Render a tokenized line for debug purposes.
void Render(
  const struct TokenizedLine *tokenized,
  Offset line_number)
{
  printf("Line #%zu\n", line_number);
  printf("  Indent level: %zu\n", tokenized->indent_level);
  printf("  Token count: %zu\n", tokenized->tokens_w);

  for (Offset i = 0; i < tokenized->tokens_w; i++)
  {
    printf("    %s\n", tokenized->tokens[i]);
  }
}

#ifndef tokenizing_h_already_included
#define tokenizing_h_already_included

#include "common_data_types.h"
#include "memory.h"
#include "text.h"


/*
  This represents a line of code that's been lightly processed.

  Its indent level is calculated, its whitespace and commentary
  are removed, and the remaining pieces of code are collected as
  tokens.

  Given this line of code:
    "DotProduct (Vector2D, Vector2D) -> Vector2D"

  Here's the representation:
    .tokens =
    {
      "DotProduct", "(Vector2D,", "Vector2D)", "->", "Vector2D"
    },
    .tokens_s = 5,
    .indent_level = 0

  Given this line of code:
    "    return vector的x"

  Here's the representation:
    .tokens =
    {
      "return", "vector", "x"
    },
    .tokens_s = 3,
    .indent_level = 2
*/
struct TokenizedLine
{
  // If a line starts with 3 spaces, its indent level is 1.5.
  Float32 indent_level;
  Text *tokens;
  Size tokens_s;
};

constexpr auto codepoint_for_regular_space = 0x0020;
constexpr auto codepoint_for_fullwidth_space = 0x3000;
constexpr auto codepoint_for_tab = 0x0009;

constexpr auto max_line_length = 120;
constexpr auto max_tokens_per_line = max_line_length / 2;

// TODO: Figure out how to hide these constants.
/*********************************************/
constexpr auto utf8_max_character_width = FourBytesWide;
constexpr auto pointer_width = sizeof (void*);;
/*********************************************/

constexpr auto needed_memory_for_straining_a_line =
  // Memory needed for the pointers to the text of the tokens
  (max_tokens_per_line * pointer_width)
  // Memory needed for the actual text of the tokens
  + (max_line_length * utf8_max_character_width) + 1;

struct TokenizedLine TokenizedLine(
  Character line_buffer[max_line_length],
  struct Allocator *allocator);

#endif

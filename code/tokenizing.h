#ifndef tokenizing_h_already_included
#define tokenizing_h_already_included

#include "common_data_types.h"
#include "memory.h"
#include "text.h"


constexpr auto utf_codepoint_for_regular_space = 0x0020;
constexpr auto utf_codepoint_for_fullwidth_space = 0x3000;
constexpr auto utf_codepoint_for_tab = 0x0009;

/*
  This represents a line of code that's been lightly processed.

  Its indent level is calculated, its whitespace and commentary
  are removed, and the remaining interleaved pieces of code are
  collected as tokens.

  Given this line of code:
    "Vector2D DotProduct(Vector2D, Vector2D)"

  Here's the representation:
    .tokens =
    {
      "Vector2D",
      "DotProduct(Vector2D,",
      "Vector2D)"
    },
    .tokens_w = 5,
    .indent_level = 0

  Given this line of code:
    "    return vector的x"

  Here's the representation:
    .tokens =
    {
      "return",
      "vector",
      "x"
    },
    .tokens_w = 3,
    .indent_level = 2
*/
struct TokenizedLine
{
  Size indent_level;
  Text *tokens;
  Size tokens_w;
};

constexpr Size max_line_length = 250;
constexpr Size max_tokens_per_line = max_line_length / 2;

constexpr Size bytes_needed_to_tokenize_a_line =
  // Memory needed for the pointers to the text of the tokens
  (max_tokens_per_line * pointer_width)
  // Memory needed for the actual text of the tokens
  + (max_line_length * utf8_max_character_width) + 1;

struct TokenizedLine TokenizedLine(
  Character line_buffer[max_line_length],
  Size line_buffer_w,
  struct Allocator *allocator);

#endif

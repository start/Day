#ifndef chunking_h_already_included
#define chunking_h_already_included

#include "common_data_types.h"
#include "memory.h"

// This is not unreasonable.
constexpr auto max_chunks_per_line = 30;

/*
  This represents a line of code that's been lightly processed.

  Its indent level is calculated, its whitespace and commentary
  are removed, and the remaining chunks of code are preserved.

  Given this line of code:
    "DotProduct [Vector2D, Vector2D] -> Vector2D"

  Here's the representation:
    .code_chunks =
    {
      "DotProduct", "[Vector2D,", "Vector2D]", "->", "Vector2D"
    },
    .code_chunks_s = 5,
    .indent_level = 0

  Given this line of code:
    "    give vector的x"

  Here's the representation:
    .code_chunks =
    {
      "give", "vector", "x"
    },
    .code_chunks_s = 3,
    .indent_level = 2
*/
struct ChunkedLineOfCode
{
  /*
    This field represents how many levels deep this line is
    indented. So why is it a floating point number?

    To indent code by 1 level, we can use either:

    * 2 regular spaces
    * 1 fullwidth space (https://unicode-explorer.com/c/3000)
    * 1 tab

    Now, imagine an indent of 3 regular spaces. That lies between
    indent levels of 1 and 2, so which indent level should Day
    use? During this preliminary stage of processing, we'll just
    record the indent level as 1.5.

    However, ultimately, indent levels need to be whole numbers!
    A later stage of processing will be able to reject this line
    and offer a helpful error message.
  */
  Float32 indent_level;

  // A sequence of chunks of code that were delineated by either
  // whitespace or commentary.
  Text *code_chunks;

  // How many chunks of code are there?
  Size code_chunks_s;
};

constexpr auto regular_space_codepoint = 0x0020;
constexpr auto fullwidth_space_codepoint = 0x3000;
constexpr auto tab_codepoint = 0x0009;

struct ChunkedLineOfCode ChunkedLineOfCode(
  Text line_of_code,
  struct MemoryAllocator *allocator);

#endif

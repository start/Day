#include "chunking.h"
#include "basic_types.h"
#include "memory.h"
#include "utf8.h"

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
  Text *code_chunks;
  Size code_chunks_s;

  /*
    This field represents how many levels deep this line is indented.
    So Why is this field a floating point number?

    To indent code by 1 level, we can use either:

    * 2 regular spaces
    * 1 fullwidth space (https://unicode-explorer.com/c/3000)
    * 1 tab

    Okay, but what happens if someone idents a line using an odd
    number of regular spaces?

    For example, imagine an indent of 3 regular spaces. That lies
    between indent levels of 1 and 2, so which indent level
    should we choose?

    During this preliminary stage of processing, we'll just
    record the indent level as 1.5.

    However, ultimately, indent levels need to be whole numbers!
    A later stage of processing will be able to reject this line
    and offer a helpful error message.
  */
  Float32 indent_level;
};

constexpr auto regular_space_codepoint = 0x0020;
constexpr auto fullwidth_space_codepoint = 0x3000;
constexpr auto tab_codepoint = 0x0009;

YesNo IsWhitespaceOrCommentary(UTFCodepoint codepoint)
{
  return
       (codepoint == regular_space_codepoint)
    || (codepoint == fullwidth_space_codepoint)
    || (codepoint == tab_codepoint)
    || IsUTFCodepointChinese(codepoint);
}

// Note: This function isn't nearly finished!
struct ChunkedLineOfCode ChunkedLineOfCode(
  Text line_of_code,
  struct MemoryAllocator* allocator)
{
  struct ChunkedLineOfCode result =
  {
    .code_chunks = NextAddressToAllocate(allocator)
  };

  // How many bytes of this line of code have we read?
  Offset line_of_code_o = 0;

  // What's our current goal?
  enum
  {
    CalculateIndentLevel,
    FindEndOfCurrentCodeChunk,
    FindStartOfNextCodeChunk,
  } current_goal = CalculateIndentLevel;

  // If we're within a code chunk, where did it begin?
  Offset code_chunk_start_o = 0;

  // Which character are we currently looking at?
  Character character;

  /*
    In C, every string of text ends with a special character
    called a "null terminator", written as '\0'.

    On the next line, we update the current character, and then
    we check whether its new value equals the null terminator. If
    so, we terminate the loop.
  */
  while ('\0' != (character = line_of_code[line_of_code_o]))
  {
    const auto character_bundle = &character;
    const auto character_bundle_s = Utf8CharacterWidth(character_bundle);

    // Advance our offset by the width of the current character.
    line_of_code_o += character_bundle_s;

    const auto codepoint =
      UTF8Codepoint(character_bundle, character_bundle_s);

    const auto is_character_chinese =
      IsUTFCodepointChinese(codepoint);

    // If we're still calculating the indent level...
    if (current_goal == CalculateIndentLevel)
    {
      // Let's check for whitespace!
      switch (codepoint)
      {
        case regular_space_codepoint:
        {
          // Let's increase the indent level by half.
          result.indent_level += 0.5f;
          continue;
        }

        case fullwidth_space_codepoint:
        case tab_codepoint:
        {
          // Let's increase the indent level by half.
          result.indent_level += 0.5f;
          continue;
        }

        default:
        {
          // We found a different character! That means we've
          // finished calculating the indent level.
          current_goal = FindStartOfNextCodeChunk;

          /*
            But before we move on...

            We've just found the first (non-indent) character on
            this line of code.

            If this first character is Chinese, the whole line is
            considered commentary. We've gotta check for that.
          */
          if (is_character_chinese == true)
          {
            /*
              Yep. We'll treat the whole line as commentary.

              (Commentary is ignored by Day's compiler, so we
              treat the line as though it were empty.)
            */
            return (struct ChunkedLineOfCode) {};
          }
        }
      }
    }

    /*
      If we've made it here, we've already calculated the indent
      level.
    */

    if (IsWhitespaceOrCommentary(codepoint) == true)
    {
      switch (current_goal)
      {
        case FindStartOfNextCodeChunk:
        {
          // We're looking for the next chunk of code, and this
          // isn't it!
          continue;
        }

        case FindEndOfCurrentCodeChunk:
        {
          // We've found the end of the current chunk.
          // TODO: Add chunk

          current_goal = FindStartOfNextCodeChunk;
          continue;
        }

        default: unreachable();
      }
    }

    /*
      Good news, everyone! If we've made it here, the current
      character is considered actual code!
    */

    switch (current_goal)
    {
      case FindEndOfCurrentCodeChunk:
      {
        // We're looking for the end of the current chunk of
        // code, and this isn't it.
        continue;
      }

      case FindStartOfNextCodeChunk:
      {
        // We've found the start of the next chunk of code.
        code_chunk_start_o = line_of_code_o;
        current_goal = FindEndOfCurrentCodeChunk;

        continue;
      }

      default: unreachable();
    }
  }

  return result;
}
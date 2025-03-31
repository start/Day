#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#include "chunking.h"
#include "common_data_types.h"
#include "memory.h"
#include "utf8.h"

YesNo IsWhitespaceOrCommentary(UTFCodepoint codepoint)
{
  return
       (codepoint == regular_space_codepoint)
    || (codepoint == fullwidth_space_codepoint)
    || (codepoint == tab_codepoint)
    || IsUTFCodepointChinese(codepoint);
}

void ExtractAndAppendChunk(
  // The chunked line of code we're appending to.
  struct ChunkedLineOfCode* chunked_line,
  // The original line of code we're extracting from.
  Text original_line,
  // The offset of the first character in the chunk.
  Offset chunk_start_o,
  // The offset immediately following the final character in the
  // chunk.
  Offset immediately_following_chunk_end_o,
  // Our trusty allocator!
  struct MemoryAllocator *allocator)
{
  // How wide is the chunk we're extracting?
  const auto chunk_s =
    immediately_following_chunk_end_o - chunk_start_o;

  // This is where we'll store our newly extracted chunk of text.
  const OverwritableText chunk_to_extract_into =
    AllocateMemory(allocator, chunk_s + 1/*[?]*/);
  /*
    [?]: Why don't we simply allocate bytes equal to the size of
         the chunk? Why do we need add 1 to its size?

    We need to make room for the null terminator byte, '\0',
    which we'll manually tack on the end of the chunk after
    extracting it from the source line.
  */

  const auto chunk_to_extract_from =
    original_line + chunk_start_o;

  // Extract the chunk!
  memcpy(
    chunk_to_extract_into,
    chunk_to_extract_from,
    chunk_s);

  const auto chunk_still_needing_null_terminator =
    (OverwritableText) chunked_line->code_chunks[chunked_line->code_chunks_s];

  // The cherry on top.
  chunk_still_needing_null_terminator[chunk_s] = '\0';

  // Append our extracted chunk!
  chunked_line->code_chunks[chunked_line->code_chunks_s] =
    chunk_to_extract_into;

  // Let's make it official.
  chunked_line->code_chunks_s += 1;
}

// Note: This constructor isn't nearly finished!
struct ChunkedLineOfCode ChunkedLineOfCode(
  Text line_of_code,
  struct MemoryAllocator *allocator)
{
  /*
    This is what we'll return at the end, unless:

    1. The line has no code on it.
    2. The line is too long.
    3. The line has too many chunks.
  */
  struct ChunkedLineOfCode result =
  {
    .code_chunks = NextAddressToAllocate(allocator)
  };

  // How many bytes of code have we read?
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

    We lean on this to determine when terminate our loop!

    On the next line, we update the current character, then we
    check whether the character is a null terminator. If so, we
    terminate the loop.
  */
  while ('\0' != (character = line_of_code[line_of_code_o]))
  {
    const auto character_bundle = &character;
    const auto character_bundle_s = Utf8CharacterWidth(character_bundle);

    /*
      Advance our offset by the width of the current character.
      Afterward, our offset will point to the byte immediately
      following the UTF-8 character "bundle" we're examining.
    */
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
          result.indent_level += 1;
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
          // We've found the end of the current chunk! Let's add
          // it to our collection.
          ExtractAndAppendChunk(
            &result,
            line_of_code,
            code_chunk_start_o,
            line_of_code_o, // TODO: Explain
            allocator);

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
        // code, and this code character definitely isn't it.
        continue;
      }

      case FindStartOfNextCodeChunk:
      {
        // We've found the start of the next chunk of code!
        // Do we have room?
        if (result.code_chunks_s == max_chunks_per_line)
        {
          // For now, we'll throw a fit.
          exit(EXIT_FAILURE);

          // TODO: HANDLE
        }

        // TODO: Explain
        code_chunk_start_o = line_of_code_o - character_bundle_s;

        // Let's switch gears.
        current_goal = FindEndOfCurrentCodeChunk;

        continue;
      }

      default: unreachable();
    }
  }

  if (current_goal == FindEndOfCurrentCodeChunk)
  {
    ExtractAndAppendChunk(
      &result,
      line_of_code,
      code_chunk_start_o,
      line_of_code_o,
      allocator);
  }

  return result;
}
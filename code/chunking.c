#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "chunking.h"
#include "common_data_types.h"
#include "text.h"
#include "memory.h"


YesNo IsWhitespaceOrCommentary(UTFCodepoint codepoint);

// Note: This constructor isn't nearly finished!
struct ChunkedLineOfCode ChunkedLineOfCode(
  Text line_of_code,
  struct MemoryAllocator *allocator)
{
  OverwritableText code_chunks[max_chunks_per_line];
  auto code_chunks_s = 0;
  auto indent_level = 0.0f;

  // Where is the next character we're going to examine?
  Offset next_character_o = 0;

  // What's our current goal?
  enum
  {
    CalculateIndentLevel,
    FindEndOfCurrentCodeChunk,
    FindStartOfNextCodeChunk,
  } current_goal = CalculateIndentLevel;

  // If we're within a code chunk, where did it begin?
  Offset chunk_start_o = 0;

  // Which character are we examining?
  Character character;

  /*
    In C, every string of text ends with a special character
    called a "null terminator", written as '\0'.

    We lean on this to determine when terminate our loop!

    On the next line, we update the current character, then we
    check whether the character is a null terminator. If so, we
    terminate the loop.
  */
  while ('\0' != (character = line_of_code[next_character_o]))
  {
    const auto character_bundle = &character;
    const auto character_bundle_s = Utf8CharacterWidth(character_bundle);

    // Let's save the offset of the current character. We might
    // need this later.
    const auto character_o = next_character_o;

    /*
      With that saved, let's advance our character offset by the
      width of the current character.

      Afterward, our offset will point to the byte immediately
      following the current UTF-8 character "bundle" we're
      examining. We'll be ready for the next loop iteration!
    */
    next_character_o += character_bundle_s;

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
          indent_level += 0.5f;
          continue;
        }

        case fullwidth_space_codepoint:
        case tab_codepoint:
        {
          indent_level += 1;
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
          // If we're looking for the next chunk of code, this
          // isn't it!
          continue;
        }

        case FindEndOfCurrentCodeChunk:
        {
          // We've found the end of the current chunk! Let's add
          // it to our collection.

          //
          //
          /*
            We've already advanced 'next_character_o' beyond the
            current character. It now points to the next one!


          */
          const auto just_after_chunk_end =
            character_o - character_bundle_s;

          // Extract the chunk...
          const auto chunk = CopyText(
            line_of_code,
            chunk_start_o,
            just_after_chunk_end,
            allocator);

          // ... and make it official!
          code_chunks[code_chunks_s] = (OverwritableText) chunk;
          code_chunks_s += 1;

          current_goal = FindStartOfNextCodeChunk;
          continue;
        }

        // Nothing to see here.
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
        // If we're looking for the end of the current chunk of
        // code, then this code character isn't it.
        continue;
      }

      case FindStartOfNextCodeChunk:
      {
        // We've found the start of the next chunk of code!
        // Do we have room?
        if (code_chunks_s == max_chunks_per_line)
        {
          // For now, we'll just throw a fit.
          exit(EXIT_FAILURE);
        }

        // TODO: Explain
        chunk_start_o = next_character_o - character_bundle_s;

        // Let's switch gears.
        current_goal = FindEndOfCurrentCodeChunk;
        continue;
      }

      // That's impossible!
      default: unreachable();
    }
  }

  // If we were in the middle of a chunk when we reached the null
  // terminator byte.
  if (current_goal == FindEndOfCurrentCodeChunk)
  {
    // Then let's collect the chunk and head home!

    // We know 'next_character_o' points to the null terminator
    // byte, which is indeed just after our chunk's end!
    const auto just_after_chunk_end = next_character_o;

    const auto chunk = CopyText(
      line_of_code,
      chunk_start_o,
      just_after_chunk_end,
      allocator);

    code_chunks[code_chunks_s] = (OverwritableText) chunk;
    code_chunks_s += 1;
  }

  return (struct ChunkedLineOfCode)
  {
    .indent_level = indent_level,
    // We aren't copying the code chunks themselves, but instead
    // the collection that points to 'em.
    .code_chunks = AllocateCopy(allocator, code_chunks, code_chunks_s),
    .code_chunks_s = code_chunks_s
  };
}

/*
  Does this codepoint represent either whitespace or commentary,
  as far as Day's rules are concerned?
*/
YesNo IsWhitespaceOrCommentary(UTFCodepoint codepoint)
{
  return
    // Is it whitespace?
       (codepoint == regular_space_codepoint)
    || (codepoint == fullwidth_space_codepoint)
    || (codepoint == tab_codepoint)
    // Is it commentary?
    || IsUTFCodepointChinese(codepoint);
}

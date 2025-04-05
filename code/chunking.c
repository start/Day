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
  Character line_buffer[static max_line_length],
  struct Allocator *allocator)
{
  Size code_chunks_s = 0;
  auto indent_level = 0.0f;

  // Literally——
  constexpr auto max_chunks_per_line = max_line_length / 2;
  OverwritableText code_chunks[max_chunks_per_line];

  // Where is the next character we're going to examine?
  Offset next_character_o = 0;

  // Was our line truncated because it's too long? (We assume the
  // worst until proven otherwise.)
  YesNo is_line_truncated = true;

  // What's our current goal?
  enum
  {
    CalculateIndentLevel,
    FindEndOfCurrentCodeChunk,
    FindStartOfNextCodeChunk,
  } goal = CalculateIndentLevel;

  // If we're within a code chunk, where did it begin?
  Offset chunk_start_o = 0;

  // Which character are we examining?
  Character character;

  while ('\0' != (character = line_buffer[next_character_o]))
  {
    if (character == '\n')
    {
      is_line_truncated = false;
      break;
    }

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

    const auto character_codepoint =
      UTF8Codepoint(character_bundle, character_bundle_s);

    const auto is_character_chinese =
      IsUTFCodepointChinese(character_codepoint);

    // If we're still calculating the indent level...
    if (goal == CalculateIndentLevel)
    {
      // Let's check for whitespace!
      switch (character_codepoint)
      {
        case codepoint_for_fullwidth_space:
        case codepoint_for_tab:
        {
          indent_level += 1;
          continue;
        }

        case codepoint_for_regular_space:
        {
          indent_level += 0.5f;
          continue;
        }

        default:
        {
          // We found something other than indentation. That
          // means we've finished calculating the indent level.
          goal = FindStartOfNextCodeChunk;

          /*
            But before we move on...

            We've just found the first (non-indent) character on
            this line of code, right?

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

    if (IsWhitespaceOrCommentary(character_codepoint) == true)
    {
      switch (goal)
      {
        case FindStartOfNextCodeChunk:
        {
          // If we're looking for the next chunk of code, this
          // isn't it!
          continue;
        }

        case FindEndOfCurrentCodeChunk:
        {
          //  We've been hunting for the end of the current chunk
          //  of code, and here it is.

          // Let's copy it...
          const auto chunk = CopyText(
            line_buffer,
            chunk_start_o,
            character_o,
            allocator);

          // ... and make it official!
          code_chunks[code_chunks_s] = (OverwritableText) chunk;
          code_chunks_s += 1;

          goal = FindStartOfNextCodeChunk;
          continue;
        }

        // Nothing to see here.
        default: unreachable();
      }
    }

    /*
      Good news, everyone! If we've made this far, the current
      character is considered actual code.
    */

    switch (goal)
    {
      case FindEndOfCurrentCodeChunk:
      {
        // Since we're looking for the end of the current chunk
        // of code, this code character isn't it. Let's move on.
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

        // Let's record the start of our chunk...
        chunk_start_o = character_o;
        // ... and switch gears.
        goal = FindEndOfCurrentCodeChunk;

        continue;
      }

      // That's impossible!
      default: unreachable();
    }
  }

  if (is_line_truncated == true)
  {
    // For now, let's throw a fit.
    exit(EXIT_FAILURE);
  }

  // If we were in the middle of a chunk when we reached the null
  // terminator byte...
  if (goal == FindEndOfCurrentCodeChunk)
  {
    // ...then let's collect the chunk and head home.

    // We know 'next_character_o' points just past the end of the
    // line.
    const auto just_after_chunk_end = next_character_o;

    // Extract the chunk...
    const auto chunk = CopyText(
      line_buffer,
      chunk_start_o,
      just_after_chunk_end,
      allocator);

    // ... and make it official!
    code_chunks[code_chunks_s] = (OverwritableText) chunk;
    code_chunks_s += 1;
  }

  return (struct ChunkedLineOfCode)
  {
    .indent_level = indent_level,
    .code_chunks_s = code_chunks_s,
    // We aren't copying chunks' text; we're copying pointers to
    // those pieces of text.
    .code_chunks = AllocateCopy(
      allocator,
      code_chunks,
      code_chunks_s * sizeof code_chunks[0]),
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
       (codepoint == codepoint_for_regular_space)
    || (codepoint == codepoint_for_fullwidth_space)
    || (codepoint == codepoint_for_tab)
    // Is it commentary?
    || IsUTFCodepointChinese(codepoint);
}

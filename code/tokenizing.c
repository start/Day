#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "tokenizing.h"
#include "common_data_types.h"
#include "text.h"
#include "memory.h"


YesNo IsWhitespaceOrCommentary(UTFCodepoint codepoint);


// Note: This constructor isn't nearly finished!
struct TokenizedLine TokenizedLine(
  // A character buffer starting with our null-terminated line.
  Character line_buffer[static max_line_length],
  struct Allocator *allocator)
{
  // TODO: Handle quotations.

  // Should we use a dummy 'TokenizedLine' for this?
  OverwritableText code_tokens[max_tokens_per_line];
  Size code_tokens_w = 0;
  auto indent_level = 0.0f;

  // Where is the next character we're going to examine?
  Offset next_character_o = 0;

  // What's our current goal?
  enum
  {
    CalculateIndentLevel,
    FindStartOfNextToken,
    FindEndOfCurrentToken,
    FindEndOfQuotation
  } goal = CalculateIndentLevel;

  // If we're within a code token, where did it begin?
  Offset token_start_o = 0;

  // Which character are we examining?
  Character character;

  while ('\0' != (character = line_buffer[next_character_o]))
  {
    auto character_bundle = &character;
    auto character_bundle_w = UTF8CharacterWidth(character_bundle);

    // Let's save the offset of the current character. We might
    // need this later.
    auto character_o = next_character_o;

    /*
      With that saved, let's advance our character offset by the
      width of the current character.

      Afterward, our offset will point to the byte immediately
      following the current UTF-8 character "bundle" we're
      examining. We'll be ready for the next loop iteration!
    */
    next_character_o += character_bundle_w;

    // TODO: Does T need a max line length?
    if (next_character_o >= max_line_length)
    {
      fprintf(
        stderr,
        "The maximum line length is %i. This line is longer:\n%s",
        max_line_length, line_buffer);

      // TODO: Record the error and move along.
      exit(EXIT_FAILURE);
    }

    auto character_codepoint =
      UTF8Codepoint(character_bundle, character_bundle_w);

    auto is_character_chinese =
      IsUTFCodepointChinese(character_codepoint);

    // If we're still calculating the indent level...
    if (CalculateIndentLevel == goal)
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
          goal = FindStartOfNextToken;

          /*
            But before we move on...

            We've just found the first (non-indent) character on
            this line of code, right?

            If this first character is Chinese, the whole line is
            considered commentary. We've gotta check for that.
          */
          if (is_character_chinese == true)
          {
            // Yep. We'll treat the whole line as commentary.
            return (struct TokenizedLine) {};
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
        case FindStartOfNextToken:
        {
          // If we're looking for the next token of code, this
          // isn't it!
          continue;
        }

        case FindEndOfCurrentToken:
        {
          //  We've been hunting for the end of the current token
          //  of code, and here it is.

          // Let's copy it...
          auto token = CopyText(
            line_buffer,
            token_start_o,
            character_o,
            allocator);

          // ... and make it official!
          code_tokens[code_tokens_w] = (OverwritableText) token;
          code_tokens_w += 1;

          goal = FindStartOfNextToken;
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
      case FindEndOfCurrentToken:
      {
        // Since we're looking for the end of the current token
        // of code, this code character isn't it. Let's move on.
        continue;
      }

      case FindStartOfNextToken:
      {
        // We've found the start of the next token. Let's record
        // it...
        token_start_o = character_o;
        // ... and switch gears.
        goal = FindEndOfCurrentToken;

        continue;
      }

      // That's impossible!
      default: unreachable();
    }
  }

  // If we were in the middle of a token when we reached the null
  // terminator byte...
  if (FindEndOfCurrentToken == goal)
  {
    // ...then let's collect the token and head home.

    // We know 'next_character_o' points just past the end of the
    // line.
    auto just_after_token_end = next_character_o;

    // Extract the token...
    auto code_token = CopyText(
      line_buffer,
      token_start_o,
      just_after_token_end,
      allocator);

    // ... and make it official!
    code_tokens[code_tokens_w] = (OverwritableText) code_token;
    code_tokens_w += 1;
  }

  return (struct TokenizedLine)
  {
    .indent_level = indent_level,
    .tokens_w = code_tokens_w,
    // We aren't copying tokens' text; we're copying pointers to
    // those pieces of text.
    .tokens = AllocateCopy(
      allocator,
      code_tokens,
      code_tokens_w * sizeof code_tokens[0]),
  };
}

/*
  Does this codepoint represent either whitespace or commentary,
  as far as T's rules are concerned?
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

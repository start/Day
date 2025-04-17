#include "tokenizing.h"
#include "common_data_types.h"
#include "text.h"
#include "memory.h"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "exit_with_error.h"


YesNo IsWhitespaceOrCommentary(UTFCodepoint codepoint);


/*
  This constructor produces a TokenizedLine, given a line of code
  and an allocator.

  To keep things beautifully simple, if we encounter any syntax
  errors, we report the error to the standard error stream, then
  we safely exit the program.
*/
struct TokenizedLine TokenizedLine(
  // A character buffer starting with our null-terminated line.
  Character line_buffer[static max_line_length],
  // Which line are we on?
  Size line_number,
  // Our trusty allocator.
  struct Allocator *allocator)
{
  // Every token we find goes here.
  OverwritableText code_tokens[max_tokens_per_line];

  // How many tokens have we found?
  Size code_tokens_w = 0;

  // A regular space increments this by 1; a full-width space or
  // a tab increments this by 2.
  Size spaces_of_indentation_w = 0.0f;

  // Where is the next character we're going to examine?
  Offset next_character_o = 0;

  // What's our current goal?
  enum
  {
    CalculateIndentLevel,
    FindStartOfNextToken,
    FindEndOfCurrentToken,
    // TODO: Handle quotations.
    FindEndOfQuotation
  } goal = CalculateIndentLevel;

  // If we're within a code token, where did it begin?
  Offset token_start_o = 0;

  // Which character are we examining?
  Character character;

  // "If this character isn't the null terminator byte..."
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

    if (next_character_o >= max_line_length)
    {
      ExitWithError(
        "Line number: %zu\n"
        "The maximum line length is %i.\n",
        line_number,
        max_line_length);
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
        case utf_codepoint_for_regular_space:
        {
          spaces_of_indentation_w += 1;
          continue;
        }

        case utf_codepoint_for_fullwidth_space:
        case utf_codepoint_for_tab:
        {
          spaces_of_indentation_w += 2;
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
            considered commentary. Let's check for that.
          */
          if (is_character_chinese == true)
          {
            // Yep. We'll treat the whole line as commentary.
            return (struct TokenizedLine) {};
          }

          // T uses 2 spaces to indicate each indent level.
          if ((spaces_of_indentation_w % 2) == 1)
          {
            // This line is indented with an odd number of
            // spaces. That's a mistake.
            ExitWithError(
              "Line number: %zu\n"
              "The indent level of this line is %zu spaces. "
              "It must be a multiple of two.",
              line_number,
              spaces_of_indentation_w);
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

  // (Here, we're outside the main loop.)

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

    // Show that we're finished with the final token.
    goal = FindStartOfNextToken;
  }

  // (Here, we're at the very end of our constructor.)

  switch (goal)
  {
    // If we're still trying to calculate the indent level...
    case CalculateIndentLevel:
    {
      // ... then this line is empty or whitespace.
      return (struct TokenizedLine) {};
    }

    // If we're looking for the start of the next token...
    case FindStartOfNextToken:
    {
      // ... that means we found 1 or more tokens, and we aren't
      // in the middle of anything. We're done!
      return (struct TokenizedLine)
      {
        .indent_level = spaces_of_indentation_w / 2,
        .tokens_w = code_tokens_w,
        // We aren't copying tokens' text; we're copying pointers
        // to those pieces of text.
        .tokens = AllocateCopy(
          allocator,
          code_tokens,
          code_tokens_w * sizeof code_tokens[0]),
      };
    }

    case FindEndOfQuotation:
    {
      ExitWithError(
        "Line number: %zu\n"
        "Line ended in the middle of a quotation.\n",
        line_number);
    }

    // We handled this directly before this 'switch'.
    case FindEndOfCurrentToken: unreachable();
  }
}


/*
  Does this codepoint represent either whitespace or commentary,
  as far as T's rules are concerned?
*/
YesNo IsWhitespaceOrCommentary(UTFCodepoint codepoint)
{
  return
    // Is it whitespace?
       (codepoint == utf_codepoint_for_regular_space)
    || (codepoint == utf_codepoint_for_fullwidth_space)
    || (codepoint == utf_codepoint_for_tab)
    // Is it commentary?
    || IsUTFCodepointChinese(codepoint);
}

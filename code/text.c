#include <stdlib.h>
#include "text.h"
#include "common_data_types.h"
#include "memory.h"


// Copy a snippet of text.
Text CopyText(
  // The original line of code we're extracting from.
  Text full_text,
  // The offset of the first character we're copying.
  Offset snippet_start_o,
  // The offset immediately following the final character to copy.
  Offset just_after_snippet_end_o,
  // Our trusty allocator!
  struct Allocator *allocator)
{
  // Where is the snippet we'll be copying?
  auto copy_from = full_text + snippet_start_o;

  // How wide is the snippet we're copying?
  auto snippet_s =
    just_after_snippet_end_o - snippet_start_o;

  // Copy it.
  OverwritableText copied_snippet =
    AllocateCopy(allocator, (Memory) copy_from, snippet_s);

  // The cherry on top! Let's add the null terminator byte.
  Allocate(allocator, 1);
  copied_snippet[snippet_s] = '\0';

  return copied_snippet;
}


// How many bytes wide is this UTF-8 character?
enum UTF8CharacterWidth Utf8CharacterWidth(const CharacterBundle character_bundle)
{
  auto first_byte = *character_bundle;

  if ((first_byte & 0b1000'0000) == 0)
  {
    // The first bit is 0.
    return OneByteWide;
  }

  if ((first_byte & 0b1110'0000) == 0b1100'0000)
  {
    // The first 2 bits are 1; the 3rd is 0.
    return TwoBytesWide;
  }

  if ((first_byte & 0b1111'0000) == 0b1110'0000)
  {
    // The first 3 bits are 1; the 4th is 0.
    return ThreeBytesWide;
  }

  if ((first_byte & 0b1111'1000) == 0b1111'0000)
  {
    // The first 4 bits are 1; the 5th is 0.
    return FourBytesWide;
  }

  // Invalid UTF-8. Who invited this character, anyway?
  exit(EXIT_FAILURE);
}


// What is the UTF codepoint of this UTF-8 character?
UTFCodepoint UTF8Codepoint(
  CharacterBundle character_bundle,
  enum UTF8CharacterWidth character_bundle_s)
{
  /*
    UTF-8 characters can consist of 1, 2, 3, or 4 bytes.

    1 byte:                             0xxxxxxx
    2 bytes:                   110xxxxx 10xxxxxx
    3 bytes:          1110xxxx 10xxxxxx 10xxxxxx
    4 bytes: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

    For any character of a given length, the leading 1's and 0's
    are identical.

    For example, all 2-byte characters have their first byte
    start with '110' and their second start with '10'.

    That leaves the x's! Those x's, collectively, represent the
    codepoint. To determine the codepoint, we simply squish all
    the x's together!
  */

  switch (character_bundle_s)
  {
    case 1:
    {
      // 1-byte characters are easy.
      //
      // As you can see above, the 7 x's for 1-byte characters
      // are already squished together, exactly where we need
      // 'em.
      return *character_bundle;
    }

    case 2:
    {
      auto byte_1_xs = 0b0001'1111 & character_bundle[0];
      auto byte_2_xs = 0b0011'1111 & character_bundle[1];

      /*
        Below, we shift the first byte's x's 6 bits to the left,
        because we need to make room for the 6 x's of the second
        byte.

        Then, using '|', we "merge" the x's from both bytes
        without any overlap.

        Does that make some sense?
      */
      return (byte_1_xs << 6) | byte_2_xs;
    }

    case 3:
    {
      auto byte_1_xs = 0b0000'1111 & character_bundle[0];
      auto byte_2_xs = 0b0011'1111 & character_bundle[1];
      auto byte_3_xs = 0b0011'1111 & character_bundle[2];

      /*
        The same principle applies here.

        However, this time, we need to shift the first byte's
        x's 12 bits to the left, because we need to make room
        for the 12 total x's from the second and third bytes.
      */
      return
          byte_1_xs << 12
        | byte_2_xs << 6
        | byte_3_xs;
    }

    case 4:
    {
      auto byte_1_xs = 0b0000'0111 & character_bundle[0];
      auto byte_2_xs = 0b0011'1111 & character_bundle[1];
      auto byte_3_xs = 0b0011'1111 & character_bundle[2];
      auto byte_4_xs = 0b0011'1111 & character_bundle[3];

      return
          byte_1_xs << 18
        | byte_2_xs << 12
        | byte_3_xs << 6
        | byte_4_xs;
    }

    default:
    {
      // Invalid UTF-8.
      exit(EXIT_FAILURE);
    }
  }
}


// Does this UTF codepoint represent a Chinese character?
YesNo IsUTFCodepointChinese(UTFCodepoint code)
{
  // https://www.unicode.org/charts/
  return
       (code >= 0x03000 && code <= 0x0303F) // CJK Symbols and Punctuation
    || (code >= 0x04E00 && code <= 0x09FFF) // CJK Unified Ideographs (Han)
    || (code >= 0x03400 && code <= 0x04DBF) // CJK Extension A
    || (code >= 0x20000 && code <= 0x2A6DF) // CJK Extension B
    || (code >= 0x2A700 && code <= 0x2B739) // CJK Extension C
    || (code >= 0x2B740 && code <= 0x2B81D) // CJK Extension D
    || (code >= 0x2B820 && code <= 0x2CEA1) // CJK Extension E
    || (code >= 0x2CEB0 && code <= 0x2EBE0) // CJK Extension F
    || (code >= 0x30000 && code <= 0x3134A) // CJK Extension G
    || (code >= 0x31350 && code <= 0x323AF) // CJK Extension H
    || (code >= 0x2EBF0 && code <= 0x2EE5D) // CJK Extension I
    || (code == 0x0FF01)                    // Fullwidth exclamation mark
    || (code == 0x0FF0C)                    // Fullwidth comma
    || (code == 0x0FF0E)                    // Fullwidth period
    || (code == 0x0FF1A)                    // Fullwidth colon
    || (code == 0x0FF1B)                    // Fullwidth semicolon
    || (code == 0x0FF1F);                   // Fullwidth question mark
}
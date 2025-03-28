#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

size_t UTF8CharacterWidth(const char *character);
uint32_t UTF8CharacterCodepoint(const char *character, const size_t width);
bool IsUTF8CharacterChinese(uint32_t codepoint);

int main(void)
{
  // "Hello, world!"
  const auto greeting = u8"你好，世界！yawn";

  // How many bytes wide is the greeting?
  const auto greeting_w = strlen(greeting);

  // The index of the current character in our greeting.
  size_t greeting_i = 0;
  while (greeting_i < greeting_w)
  {
    const auto character = greeting + greeting_i;
    const auto character_w = UTF8CharacterWidth(character);

    const auto codepoint = UTF8CharacterCodepoint(character, character_w);
    const auto is_chinese = IsUTF8CharacterChinese(codepoint);

    /*
      Example output:
      
      你      (0x00004f60)   Chinese!
      好      (0x0000597d)   Chinese!
      ，      (0x0000ff0c)   Chinese!
      世      (0x00004e16)   Chinese!
      界      (0x0000754c)   Chinese!
      ！      (0x0000ff01)   Chinese!
      y       (0x00000079)   Booooooring.
      a       (0x00000061)   Booooooring.
      w       (0x00000077)   Booooooring.
      n       (0x0000006e)   Booooooring.
    */
    printf("%.*s\t(0x%08x)   %s\n",
           (int)character_w,
           character,
           codepoint,
           is_chinese ? "Chinese!" : "Booooooring.");

    greeting_i += character_w;
  }

  return 0;
}

// How many bytes wide is this UTF-8 character?
size_t UTF8CharacterWidth(const char *character)
{
  const char first_byte = *character;

  if ((first_byte & 0b1000'0000) == 0)
  {
    // The first bit is 0.
    return 1;
  }

  if ((first_byte & 0b1110'0000) == 0b1100'0000)
  {
    // The first 2 bits are 1; the 3rd is 0.
    return 2;
  }

  if ((first_byte & 0b1111'0000) == 0b1110'0000)
  {
    // The first 3 bits are 1; the 4th is 0.
    return 3;
  }

  if ((first_byte & 0b1111'1000) == 0b1111'0000)
  {
    // The first 4 bits are 1; the 5th is 0.
    return 4;
  }

  // Invalid UTF-8. Who invited this character, anyway?
  exit(EXIT_FAILURE);
}

// What is the codepoint of this UTF-8 character?
uint32_t UTF8CharacterCodepoint(
    const char *character,
    const size_t character_w)
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

  switch (character_w)
  {
    case 1:
    {
      // 1-byte characters are easy.
      //
      // As you can see above, the 7 x's for 1-byte characters
      // are already squished together, exactly where we need
      // 'em.
      return (uint32_t)*character;
    }

    case 2:
    {
      const auto byte_1_xs = character[0] & 0b0001'1111;
      const auto byte_2_xs = character[1] & 0b0011'1111;

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
      const auto byte_1_xs = character[0] & 0b0000'1111;
      const auto byte_2_xs = character[1] & 0b0011'1111;
      const auto byte_3_xs = character[2] & 0b0011'1111;

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
      const auto byte_1_xs = character[0] & 0b0000'0111;
      const auto byte_2_xs = character[1] & 0b0011'1111;
      const auto byte_3_xs = character[2] & 0b0011'1111;
      const auto byte_4_xs = character[3] & 0b0011'1111;

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

// Is this UTF-8 character codepoint Chinese?
bool IsUTF8CharacterChinese(uint32_t codepoint)
{
  // https://www.unicode.org/charts/
  return    (codepoint >= 0x03000 && codepoint <= 0x0303F) // CJK Symbols and Punctuation
         || (codepoint >= 0x04E00 && codepoint <= 0x09FFF) // CJK Unified Ideographs (Han)
         || (codepoint >= 0x03400 && codepoint <= 0x04DBF) // CJK Extension A
         || (codepoint >= 0x20000 && codepoint <= 0x2A6DF) // CJK Extension B
         || (codepoint >= 0x2A700 && codepoint <= 0x2B739) // CJK Extension C
         || (codepoint >= 0x2B740 && codepoint <= 0x2B81D) // CJK Extension D
         || (codepoint >= 0x2B820 && codepoint <= 0x2CEA1) // CJK Extension E
         || (codepoint >= 0x2CEB0 && codepoint <= 0x2EBE0) // CJK Extension F
         || (codepoint >= 0x30000 && codepoint <= 0x3134A) // CJK Extension G
         || (codepoint >= 0x31350 && codepoint <= 0x323AF) // CJK Extension H
         || (codepoint >= 0x2EBF0 && codepoint <= 0x2EE5D) // CJK Extension I
         || (codepoint == 0x0FF01)                         // Fullwidth exclamation mark
         || (codepoint == 0x0FF0C)                         // Fullwidth comma
         || (codepoint == 0x0FF0E)                         // Fullwidth period
         || (codepoint == 0x0FF1A)                         // Fullwidth colon
         || (codepoint == 0x0FF1B)                         // Fullwidth semicolon
         || (codepoint == 0x0FF1F);                        // Fullwidth question mark
}

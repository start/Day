#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  In theory, a character is the smallest meaningful unit of text,
  like a letter or digit.

  Unfortunately, C promises to recognize only a small handful of
  distinct characters--typically just 255.

  That's a big problem: The Chinese langauge alone has tens of
  thousands of distinct characters. C characters just aren't big
  enough to represent the world's languages!

  Thus, text encodings like "UTF-8" were born. Behind the scenes,
  these encodings represent real-world characters (like 天）by
  bundling together multiple C characters.

  These bundles of C characters are big enough to represent
  over a million distinct real-world characters.

  To minimize confusion, within these bundles, each C character
  is typically referred to as a "byte" rather than a character.

  (This character type represents a C character, not a bundle.)
*/
typedef char Character;

/*
  Text is a bunch of characters strung together.

  In C, to reference "a bunch of [items] strung together", we
  typically use something called a "pointer".

  You can think of a pointer like it's a locker number. A locker
  number points to one specific locker among many lockers, right?
  Likewise, a pointer points to one specific item among many.

  In C, text is almost always represented by a character pointer
  pointing to the text's first character. To reference the text's
  second character, we add 1 to our pointer's value, and so on.

  (Likewise, we add 1 to a locker number to reference its
  neighbor.)

  Note that C pointers have many, many uses. They always point to
  something, but they don't always represent a bunch of items
  strung together.

  P.S. Below, "const" means "the text can be read, not changed."
*/
typedef const Character *Text;

// A bundle of C characters representing a real-world character
// (like 天).
typedef const Character *CharacterBundle;

// The numeric identifier, or "code", of a UTF-8 character.
typedef uint32_t UTF8Codepoint;

// Some number of items. Whole numbers only, zero or greater.
typedef size_t Count;

/*
  Given multiple items, an offset indicates which item we're
  referring to. A value of 0 indicates the first item, 1 is the
  second item, 2 is the third item, etc.

  Whole numbers only, zero or greater.

  Offsets sound a little like pointers, right? Yes, they're
  related! If we think of a pointer as representing a locker
  number, then an offset represents how many lockers to its right
  is the locker we actually want to open.

  In other words: Pointers are absolute; offsets are relative.

  Does that make some sense?
*/
typedef size_t Offset;

// The answer to a yes/no question.
typedef bool YesNo;

// Positive or negative whole numbers.
typedef int Integer;

/*
  In C, we have to announce a function's signature before we can
  use the function.

  That's what this next little block is for!
*/

Count UTF8CharacterWidth(CharacterBundle bundle);
UTF8Codepoint UTF8CharacterCodepoint(
  CharacterBundle bundle,
  Count character_w);
YesNo IsUTF8CharacterChinese(UTF8Codepoint code);

/*
  When our program runs, C calls this function. Thus, this is our
  program's "entry point".

  C is very strict about this function! It must be named "main"
  with all lowercase letters, it must return an integer, and its
  arguments must have specific types.
*/
Integer main(Integer argument_count, Text arguments[])
{
  // "Hello, world!"
  const auto greeting = u8"你好，世界！yawn";

  // How many bytes wide is the greeting?
  const auto greeting_w = strlen(greeting);

  // The offset of the current character in our greeting.
  Offset greeting_o = 0;
  while (greeting_o < greeting_w)
  {
    const auto character = greeting + greeting_o;
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
    printf(
      "%.*s\t(0x%08x)   %s\n",
      (Integer) character_w, character,
      codepoint,
      is_chinese ? "Chinese!" : "Booooooring.");

    greeting_o += character_w;
  }

  // By returning 0, we tell C "We finished successfully!"
  return 0;
}

// How many bytes wide is this UTF-8 character?
Count UTF8CharacterWidth(const CharacterBundle bundle)
{
  const auto first_byte = *bundle;

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
UTF8Codepoint UTF8CharacterCodepoint(
  const CharacterBundle bundle,
  const Count character_w)
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
      return *bundle;
    }

    case 2:
    {
      const auto byte_1_xs = 0b0001'1111 & bundle[0];
      const auto byte_2_xs = 0b0011'1111 & bundle[1];

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
      const auto byte_1_xs = 0b0000'1111 & bundle[0];
      const auto byte_2_xs = 0b0011'1111 & bundle[1];
      const auto byte_3_xs = 0b0011'1111 & bundle[2];

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
      const auto byte_1_xs = 0b0000'0111 & bundle[0];
      const auto byte_2_xs = 0b0011'1111 & bundle[1];
      const auto byte_3_xs = 0b0011'1111 & bundle[2];
      const auto byte_4_xs = 0b0011'1111 & bundle[3];

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
YesNo IsUTF8CharacterChinese(const UTF8Codepoint code)
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
    || (code == 0x0FF01)                         // Fullwidth exclamation mark
    || (code == 0x0FF0C)                         // Fullwidth comma
    || (code == 0x0FF0E)                         // Fullwidth period
    || (code == 0x0FF1A)                         // Fullwidth colon
    || (code == 0x0FF1B)                         // Fullwidth semicolon
    || (code == 0x0FF1F);                        // Fullwidth question mark
}


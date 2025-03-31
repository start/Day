#ifndef basic_types_h_already_included
#define basic_types_h_already_included

#include <stdint.h>
#include <stddef.h>

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

// Text that can be overwritten, copied into, etc.
typedef Character *OverwritableText;

// A bundle of C characters representing a real-world character,
// like "天" or "!".
typedef const Character *CharacterBundle;

/*
  The numeric identifier, or "codepoint", of a UTF character.

  Fun fact: UTF-8, UTF-16, and UTF-32 all share the same
  character codepoints! For example, an exclamation mark "!" is
  represented by a codepoint of 33 in all three.

  The difference between the three UTF encodings lies in how
  those various codepoints are "encoded" within text. (Get it??)
*/
typedef uint32_t UTFCodepoint;

// Some number of items. Whole numbers only, zero or greater.
typedef size_t Size;

/*
  A 32-bit floating point number.

  Note: As of C23, <stdfloat.h> is supposed to define floating
  point types with explicit bit widths, but <stdfloat.h> isn't
  widely supported yet.
*/
typedef float Float32;

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

// A raw "byte" of data, 8 bits wide.
typedef uint8_t Byte;

// A pointer to a raw block of memory storing an unspecified type
// of data.
typedef void *Memory;

#endif

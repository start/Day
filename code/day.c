#include <stdio.h>
#include <string.h>

#include "common_data_types.h"
#include "utf8.h"

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

  // How many bytes wide is our greeting?
  const auto greeting_s = strlen(greeting);

  // The offset of the current C character in our greeting.
  Offset greeting_o = 0;
  while (greeting_o < greeting_s)
  {
    // This bundle represents the real-world character we're
    // currently examining.
    const auto bundle = greeting + greeting_o;
    const auto bundle_s = Utf8CharacterWidth(bundle);

    const auto codepoint = UTF8Codepoint(bundle, bundle_s);
    const auto is_chinese = IsUTFCodepointChinese(codepoint);

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
      (Integer) bundle_s, bundle,
      codepoint,
      is_chinese ? "Chinese!" : "Booooooring.");

    greeting_o += bundle_s;
  }

  // By returning 0, we tell C "We finished successfully!"
  return 0;
}

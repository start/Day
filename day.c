#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t UTF8CharacterWidth(const char *character);


int main(void)
{
   // "Hello, world!"
   const auto greeting = u8"你好，世界！";

   // How many bytes wide is the greeting?
   const auto greeting_w = strlen(greeting);

   // The index of the current character in our greeting.
   size_t greeting_i = 0;
   while (greeting_i < greeting_w)
   {
      const auto character = greeting + greeting_i;
      const auto character_w = UTF8CharacterWidth(character);
      printf("%.*s\n", (int) character_w, character);

      greeting_i += character_w;
   }

   return 0;
}

// This UTF-8 character is how many bytes wide?
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


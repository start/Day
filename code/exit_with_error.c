#include "exit_with_error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// This exits the program and writes the provided error
// description to the standard error stream.
_Noreturn void ExitWithError(
  // What exactly went wrong?
  Text formatted_error_description,
  // If the error description contains any format specifiers,
  // they're bundled here.
  ...)
{
  // This represents those variadic '...' arguments.
  va_list variable_arguments;

  // The variadic arguments start after the
  // 'formatted_error_description' argument.
  va_start(variable_arguments, formatted_error_description);

  // 'vfprintf' is just like 'fprintf', but it can accept
  //  variadic arguments.
  vfprintf(stderr, formatted_error_description, variable_arguments);

  // Indicate that we're done with the variadic arguments.
  // (Secretly, I'm not entirely sure why this is necessary.)
  va_end(variable_arguments);

  // As promised, let's exit our program.
  exit(EXIT_FAILURE);
}

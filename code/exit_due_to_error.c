#include "exit_due_to_error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


/*
  This function exits the program and writes the provided error
  message to the standard error stream.

  If there's an underlying system error, this function writes
  that to the standard error stream, too.
*/
[[cold, noreturn]] void ExitDueToError(
  // What exactly went wrong?
  Text error_message_format,
  // If the error description contains any format specifiers,
  // they're bundled up here.
  ...)
{
  // This represents those variadic '...' arguments.
  va_list variable_arguments;

  // The variadic arguments start after the
  // 'error_message_format' argument.
  va_start(variable_arguments, formatted_error_description);

  // 'vfprintf' is just like 'fprintf', but it can accept
  //  variadic arguments.
  vfprintf(stderr, error_message_format, variable_arguments);

  // Indicate that we're done with the variadic arguments.
  // (Secretly, I'm not entirely sure why this is necessary.)
  va_end(variable_arguments);

  // Is there an underlying system error?
  if (errno != 0)
  {
    // Let's print it.
    fprintf(
      stderr,
      "Underlying error message: %s\n",
      strerror(errno));
  }

  // As promised, let's exit our program.
  exit(EXIT_FAILURE);
}

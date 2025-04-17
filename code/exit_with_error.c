#include "exit_with_error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


_Noreturn void ExitWithError(Text formatted_error, ...)
{
  // This represents those '...' arguments
  va_list variable_arguments;

  // The '...' arguments start after 'formatted_error'
  va_start(variable_arguments, formatted_error);

  // 'vfprintf' is 'fprintf', but it can accept '...' arguments.
  vfprintf(stderr, formatted_error, variable_arguments);

  // We're done with the '...' arguments. I'm not entirely sure
  va_end(variable_arguments);

  exit(EXIT_FAILURE);
}

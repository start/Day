#ifndef exit_due_to_error_h_already_included
#define exit_due_to_error_h_already_included

#include "text.h"

[[noreturn]] void ExitDueToError(Text error_message_format, ...);

#endif

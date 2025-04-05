cmake_minimum_required (VERSION 3.30)
project (Day C)

set (CMAKE_C_STANDARD 23)

# Require GCC.
if (NOT CMAKE_C_COMPILER_ID MATCHES "GNU")
  message (FATAL_ERROR "Day requires GCC. You're using ${CMAKE_C_COMPILER_ID}!")
endif ()

# Guess the appropriate linker flag for a 32-megabyte stack.
if (WIN32)
  set (STACK_SIZE_FLAG "/STACK:33554432")
elseif (APPLE)
  set (STACK_SIZE_FLAG "-Wl,-stack_size,0x2000000")
elseif (UNIX)
  set (STACK_SIZE_FLAG "-Wl,--stack-size=33554432")
else ()
  message (FATAL_ERROR "Unrecognized platform! We need a bigger stack!")
endif ()

set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${STACK_SIZE_FLAG}")

add_executable (
  Day
  day.c
  code/common_data_types.h
  code/memory.c
  code/memory.h
  code/straining.c
  code/straining.h
  code/text.c
  code/text.h
)

# Copy "new.day", our test source file, into our output directory.
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/new.day ${CMAKE_CURRENT_BINARY_DIR}/new.day COPYONLY)


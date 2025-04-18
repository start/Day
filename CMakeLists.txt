cmake_minimum_required (VERSION 3.30)
project (T C)

set (CMAKE_C_STANDARD 23)

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
  # The name of our target executable.
  t
  # All the source files needed by that executable.
  compiler.c
  code/common_data_types.h
  code/memory.c
  code/memory.h
  code/tokenizing.c
  code/tokenizing.h
  code/text.c
  code/text.h
  code/exit_due_to_error.c
  code/exit_due_to_error.h)

# Grab the paths of all files within "./t_samples/".
file (GLOB ALL_T_SAMPLE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/t_samples/*")

# Ensure a "t_samples/" directory exists in the output directory.
file (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/t_samples)

foreach (T_SAMPLE_FILE ${ALL_T_SAMPLE_FILES})
  # Grab the filename for this sample T file.
  get_filename_component (T_SAMPLE_FILENAME ${T_SAMPLE_FILE} NAME)

  # Copy the file into the output directory.
  configure_file (
    ${T_SAMPLE_FILE}
    ${CMAKE_CURRENT_BINARY_DIR}/t_samples/${T_SAMPLE_FILENAME}
    COPYONLY)
endforeach ()

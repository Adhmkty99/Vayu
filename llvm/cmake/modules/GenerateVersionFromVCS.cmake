# CMake script that writes version control information to a header.
#
# Input variables:
#   NAMES             - A list of names for each of the source directories.
#   <NAME>_SOURCE_DIR - A path to source directory for each name in NAMES.
#   HEADER_FILE       - The header file to write
#
# The output header will contain macros <NAME>_REPOSITORY and <NAME>_REVISION,
# where "<NAME>" is substituted with the names specified in the input variables,
# for each of the <NAME>_SOURCE_DIR given.

get_filename_component(LLVM_CMAKE_DIR "${CMAKE_SCRIPT_MODE_FILE}" PATH)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

include(VersionFromVCS)

# Handle strange terminals
set(ENV{TERM} "dumb")

set(LLVM_GIT_REVISION "" CACHE STRING
  "manually add revision.")
set(LLVM_GIT_REPOSITORY "" CACHE STRING
  "manually add repository.")
if(LLVM_GIT_REVISION)
  add_definitions(-DLLVM_GIT_REVISION="${LLVM_GIT_REVISION}")
endif()
if(LLVM_GIT_REPOSITORY)
  add_definitions(-DLLVM_GIT_REPOSITORY="${LLVM_GIT_REPOSITORY}")
endif()

function(append_info name path)
  if(LLVM_GIT_REVISION AND LLVM_GIT_REPOSITORY)
    set(revision ${LLVM_GIT_REVISION})
    set(repository ${LLVM_GIT_REPOSITORY})
  else()
    if(path)
      get_source_info("${path}" revision repository)
    endif()
  endif()
  #set(revision,"1234")
  #set(repository,"abcd")
  if(revision)
    file(APPEND "${HEADER_FILE}.tmp"
      "#define ${name}_REVISION \"${revision}\"\n")
  else()
    file(APPEND "${HEADER_FILE}.tmp"
      "#undef ${name}_REVISION\n")
  endif()
  if(repository)
    file(APPEND "${HEADER_FILE}.tmp"
      "#define ${name}_REPOSITORY \"${repository}\"\n")
  else()
    file(APPEND "${HEADER_FILE}.tmp"
      "#undef ${name}_REPOSITORY\n")
  endif()
endfunction()

foreach(name IN LISTS NAMES)
  if(NOT DEFINED ${name}_SOURCE_DIR)
    message(FATAL_ERROR "${name}_SOURCE_DIR is not defined")
  endif()
  append_info(${name} "${${name}_SOURCE_DIR}")
endforeach()

# Copy the file only if it has changed.
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${HEADER_FILE}.tmp" "${HEADER_FILE}")
file(REMOVE "${HEADER_FILE}.tmp")

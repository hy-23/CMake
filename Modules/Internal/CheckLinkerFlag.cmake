# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

include_guard(GLOBAL)
include(Internal/CheckFlagCommonConfig)
include(Internal/CheckSourceCompiles)
include(CMakeCheckCompilerFlagCommonPatterns)

cmake_policy(PUSH)
cmake_policy(SET CMP0054 NEW) # if() quoted variables not dereferenced

function(CMAKE_CHECK_LINKER_FLAG _lang _flag _var)
  # link step supports less languages than the compiler
  # so do a first check about the requested language
  if (_lang STREQUAL "ISPC")
    message (SEND_ERROR "check_linker_flag: ${_lang}: unsupported language.")
    return()
  endif()

  cmake_check_flag_common_init("check_linker_flag" ${_lang} _lang_src _lang_fail_regex)

  set(CMAKE_REQUIRED_LINK_OPTIONS "${_flag}")

  check_compiler_flag_common_patterns(_common_patterns)
  cmake_check_source_compiles(${_lang}
    "${_lang_src}"
    ${_var}
    ${_lang_fail_regex}
    ${_common_patterns}
    )

  cmake_check_flag_common_finish()
endfunction()

cmake_policy(POP)

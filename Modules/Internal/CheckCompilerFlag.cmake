# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

include_guard(GLOBAL)
include(Internal/CheckFlagCommonConfig)
include(Internal/CheckSourceCompiles)
include(CMakeCheckCompilerFlagCommonPatterns)

function(CMAKE_CHECK_COMPILER_FLAG _lang _flag _var)
  cmake_check_flag_common_init("check_compiler_flag" ${_lang} _lang_src _lang_fail_regex)

  set(CMAKE_REQUIRED_DEFINITIONS ${_flag})

  check_compiler_flag_common_patterns(_common_patterns)
  cmake_check_source_compiles(${_lang}
    "${_lang_src}"
    ${_var}
    ${_lang_fail_regex}
    ${_common_patterns}
    )

  cmake_check_flag_common_finish()
endfunction()

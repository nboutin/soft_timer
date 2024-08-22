cmake_minimum_required(VERSION 3.20)
project(timer)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(TIMER_TEST "Build Timer Tests" OFF)

include(EnableExtraCompilerWarnings)

# --- Dependencies
include(CPM)

# ---
add_library(${PROJECT_NAME} STATIC
  source/timer.c
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
  include
  PRIVATE
  include/soft_timer
)

target_compile_options(${PROJECT_NAME}
  PRIVATE
  -Wall -Werror -Wextra -Wconversion
  -Werror=missing-declarations
  $<$<BOOL:${TIMER_TEST}>:-g -O0>
)

# enable_extra_compiler_warnings(${PROJECT_NAME})
target_compile_definitions(${PROJECT_NAME}
  PRIVATE
  $<$<BOOL:${TIMER_TEST}>:static=>
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  $<$<BOOL:${TIMER_TEST}>:gcov>
)

if(CMAKE_CROSSCOMPILING)
  set(MCPU "-mcpu=Cortex-M0plus")
  set(MFLOAT_ABI "")
  set(RUNTIME_LIBRARY "--specs=nano.specs")
  set(RUNTIME_LIBRARY_SYSCALLS "--specs=nosys.specs")

  set(CMAKE_EXECUTABLE_SUFFIX ".elf")
  set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
  set(CMAKE_C_FLAGS "${MCPU} -std=gnu11 ${MFPU} ${MFLOAT_ABI} ${RUNTIME_LIBRARY} -mthumb -Wall -Werror")
  set(CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT} ${RUNTIME_LIBRARY_SYSCALLS} -Wl,-Map=test.map -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group")
  set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")
endif()

if(TIMER_TEST)
  project(${PROJECT_NAME}_test LANGUAGES C CXX)
  include(CTest)

  CPMAddPackage(
    NAME googletest
    GITHUB_REPOSITORY google/googletest
    VERSION 1.14.0
    OPTIONS
    "gtest_force_shared_crt ON"
    "INSTALL_GTEST OFF")

  add_custom_target(test_report)
  add_custom_command(TARGET test_report
    COMMENT "Generating test report"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test_report
    COMMAND ${CMAKE_COMMAND} -E make_directory html
    COMMAND junit2html.exe xml --merge html/test_report_all.xml
    COMMAND junit2html.exe --report-matrix ${CMAKE_SOURCE_DIR}/report/test_report_matrix.html html/test_report_all.xml
    VERBATIM)

  include(CodeCoverage)
  append_coverage_compiler_flags()
  setup_target_for_coverage_gcovr_html(
    NAME test_coverage
    EXECUTABLE ctest
    EXCLUDE test/* build/_deps/*
  )
  add_custom_command(TARGET test_coverage POST_BUILD
    COMMENT "Copying coverage report"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_BINARY_DIR}/test_coverage
    ${CMAKE_SOURCE_DIR}/report/test_coverage
  )

  add_subdirectory(test)
endif()

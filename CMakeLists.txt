cmake_minimum_required(VERSION 3.17)

project(fge
  VERSION 0.0.1
  DESCRIPTION "FGE - Felix's Game Engine"
  LANGUAGES CXX C)

option(PACKAGE_TESTS "Build the tests" ON)

# list(APPEND
#   CMAKE_MODULE_PATH
#   "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# Let's ensure -std=c++xx instead of -std=g++xx
set(CMAKE_CXX_EXTENSIONS OFF)

# Colored compiler output
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_compile_options (-fcolor-diagnostics)
endif ()

add_subdirectory(external)
add_subdirectory(src)

if(PACKAGE_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(tests)

  # Keep cache clean
  mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
    )

  add_custom_target(check
    COMMAND env CTEST_OUTPUT_ON_FAILURE=1 GTEST_COLOR=1 ${CMAKE_CTEST_COMMAND}
    )
endif()

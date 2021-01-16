cmake_minimum_required(VERSION 3.17)

project(fge
  VERSION 0.0.1
  DESCRIPTION "FGE - Felix's Game Engine"
  LANGUAGES CXX C)

option(PACKAGE_TESTS "Build the tests" OFF)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Let's ensure -std=c++xx instead of -std=g++xx
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Colored compiler output
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options (-fdiagnostics-color=always)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options (-fcolor-diagnostics)
  endif ()

  set(PACKAGE_TESTS ON)

endif()

if(PACKAGE_TESTS)
  include(GoogleTest)
  enable_testing()
  add_subdirectory(tests)
endif()

add_subdirectory(fge)
add_subdirectory(external)

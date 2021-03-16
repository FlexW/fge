cmake_minimum_required(VERSION 3.17)

set(PROJECT_NAME "FGE")
set(PROJECT_DESCRIPTION "Felix's Game Engine")

project(
  "${PROJECT_NAME}"
  VERSION 0.0.1
  DESCRIPTION "${PROJECT_DESCRIPTION}"
  LANGUAGES CXX C
  )

option(FGE_ENABLE_LOGGING "Enable logging" ON)
option(FGE_BUILD_TESTS "Build tests" ON)

# Let's ensure -std=c++xx instead of -std=g++xx
set(CMAKE_CXX_EXTENSIONS OFF)

# Colored compiler output
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options(-fdiagnostics-color=always)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_compile_options(-fcolor-diagnostics)
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(external)
add_subdirectory(src)

if (FGE_BUILD_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(tests)
endif()

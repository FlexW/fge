cmake_minimum_required(VERSION 3.17)

option(ENABLE_LOGGING "Enable logging" ON)

set(PROJECT_NAME "FGE")
set(PROJECT_DESCRIPTION "Felix's Game Engine")
set(GAME_EXECUTABLE_NAME "fge")

project(
  "${PROJECT_NAME}"
  VERSION 0.0.1
  DESCRIPTION "${PROJECT_DESCRIPTION}"
  LANGUAGES CXX C
)

# Let's ensure -stad=c++xx instead of -std=g++xx
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
add_subdirectory(examples)

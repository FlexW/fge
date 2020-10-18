find_package(Threads REQUIRED)

file(
  GLOB_RECURSE
  HEADER_LIST
  CONFIGURE_DEPENDS
  "*.hpp"
  )

file(
  GLOB_RECURSE
  SOURCE_LIST
  CONFIGURE_DEPENDS
  "*.cpp"
  )

add_library(engine ${SOURCE_LIST} ${HEADER_LIST})

target_link_libraries(
  engine
  PUBLIC
  ${CMAKE_DL_LIBS}
  Threads::Threads
  sol2
  lua
  glad
  glfw
  fmt::fmt
  fmt::fmt-header-only
  date
  imgui
  assimp
  glm
  stb
  )

target_compile_features(engine PUBLIC cxx_std_17)

target_compile_options(engine PRIVATE
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
  )

target_include_directories(
  engine
  PUBLIC
  .
  )

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/file/root_directory.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/file/root_directory.hpp)
  target_include_directories(engine PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
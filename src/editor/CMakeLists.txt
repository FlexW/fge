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

add_executable(editor ${SOURCE_LIST} ${HEADER_LIST})

target_link_libraries(
  editor
  PRIVATE
  engine
  BulletDynamics
  )

target_compile_features(editor PUBLIC cxx_std_17)

target_compile_options(editor PRIVATE
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
  )

target_include_directories(
  editor
  PRIVATE
  .
  )

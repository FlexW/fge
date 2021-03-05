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
  glad
  stb
  ${CONAN_LIBS}
)

target_compile_features(engine PUBLIC cxx_std_17)

# Needed for date.h to compile
target_compile_definitions(engine PUBLIC HAS_UNCAUGHT_EXCEPTIONS ON)

target_compile_options(
  engine PRIVATE
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)

target_include_directories(
  engine
  PUBLIC
  .
)

if(NOT ENABLE_LOGGING)
  target_compile_definitions(engine PRIVATE FGE_LOG_DISABLE 1)
endif()

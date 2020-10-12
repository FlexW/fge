file(
  GLOB_RECURSE
  HEADER_LIST
  CONFIGURE_DEPENDS
  "src/*.h"
  )

file(
  GLOB_RECURSE
  SOURCE_LIST
  CONFIGURE_DEPENDS
  "src/*.c"
  )

add_library(lua ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(lua PUBLIC src)

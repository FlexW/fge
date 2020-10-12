file(
  GLOB_RECURSE
  HEADER_LIST
  CONFIGURE_DEPENDS
  "*.h"
  )

file(
  GLOB_RECURSE
  SOURCE_LIST
  CONFIGURE_DEPENDS
  "*.cpp"
  )

add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)
add_library(imgui ${SOURCE_LIST} ${HEADER_LIST})
target_link_libraries(imgui glfw glad)
target_include_directories(imgui PUBLIC .)

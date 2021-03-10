function(add_example EXAMPLE_NAME SOURCE_FILES)
  add_executable(${EXAMPLE_NAME} ${SOURCE_FILES})

  target_link_libraries(${EXAMPLE_NAME} PRIVATE engine)

  target_include_directories(${EXAMPLE_NAME} PRIVATE common ${CMAKE_CURRENT_BINARY_DIR}/common)

  target_compile_options(
    ${EXAMPLE_NAME} PRIVATE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
    )
endfunction()

configure_file(common/root_directory.hpp.in common/root_directory.hpp)

add_example("cubes" "cubes/cubes.cpp")
add_example("shadow_map" "shadow_map/shadow_map.cpp")

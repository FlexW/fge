add_executable(${GAME_EXECUTABLE_NAME} main.cpp)

target_link_libraries(${GAME_EXECUTABLE_NAME} PRIVATE engine)

target_compile_options(
  ${GAME_EXECUTABLE_NAME} PRIVATE
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)

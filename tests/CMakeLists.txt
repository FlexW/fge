add_library(tests_common STATIC tests_common.hpp)

target_compile_features(tests_common PUBLIC cxx_std_17)

set_target_properties(tests_common PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(tests_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(tests_common PUBLIC gtest gmock)

target_compile_options(tests_common PRIVATE
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
  )

macro(package_add_test TESTNAME)
    # create an exectuable in which the tests will be stored
    add_executable(${TESTNAME} ${ARGN})
    # link the Google test infrastructure, mocking library, and a default main fuction to
    # the test executable.  Remove g_test_main if writing your own main function.
    target_link_libraries(${TESTNAME} engine tests_common gtest_main)
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TESTNAME}
        # set a working directory so your project root so that you can find test data via paths relative to the project root
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

package_add_test(TestEngineUtilArgsParser engine/util/test_args_parser.cpp)

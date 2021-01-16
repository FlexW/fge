file(GLOB_RECURSE FGE_SOURCES src/*.cpp)
file(GLOB_RECURSE FGE_HEADERS src/*.hpp include/*.hpp)

add_library(fge
  ${FGE_SOURCES}
  ${FGE_HEADERS}
  )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/version.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/include/version.hpp)

target_include_directories(fge
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE
  .
  )

target_compile_features(fge PUBLIC cxx_std_17)

# Docs only available if this is the main app
find_package(Doxygen)
if(Doxygen_FOUND)
  add_subdirectory(docs)
else()
  message(STATUS "Doxygen not found, not building docs")
endif()

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  FgeConfigVersion.cmake
  VERSION ${PACKAGE_VERSION}
  COMPATIBILITY AnyNewerVersion
  )

install(
  TARGETS fge
  EXPORT FgeTargets
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

install(DIRECTORY include/ DESTINATION include/fge)

install(EXPORT FgeTargets
  FILE FgeTargets.cmake
  NAMESPACE Fge::
  DESTINATION lib/cmake/fge
  )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/FgeConfig.cmake.in FgeConfig.cmake @ONLY)
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/FgeConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/FgeConfigVersion.cmake"
  DESTINATION lib/cmake/fge
  )

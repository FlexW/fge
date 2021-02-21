add_subdirectory(lua)
add_subdirectory(sol2)
add_subdirectory(glad)

# GLFW
set(GLFW_BUILD_EXAMPLES "OFF" CACHE STRING "")
set(GLFW_BUILD_TESTS "OFF" CACHE STRING "")
set(GLFW_BUILD_DOCS "OFF" CACHE STRING "")
set(GLFW_BUILD_INSTALL "OFF" CACHE STRING "")
set(BUILD_SHARED_LIBS "OFF" CACHE STRING "")
add_subdirectory(glfw)

# Fmt
add_subdirectory(fmt)

# Date
add_subdirectory(date)

# ImGui
add_subdirectory(imgui)

# Assimp
set(ASSIMP_BUILD_ZLIB "ON" CACHE STRING "")
set(ASSIMP_BUILD_ASSIMP_TOOLS "OFF" CACHE STRING "")
set(ASSIMP_BUILD_TESTS "OFF" CACHE STRING "")
set(ASSIMP_INSTALL "OFF" CACHE STRING "")
set(BUILD_SHARED_LIBS "OFF" CACHE STRING "")
add_subdirectory(assimp)

# GLM
add_subdirectory(glm)

# STB
add_subdirectory(stb)

# Bullet
set(USE_GRAPHICAL_BENCHMARK "OFF" CACHE STRING "")
set(BUILD_CPU_DEMOS "OFF" CACHE STRING "")
set(BUILD_EGL "ON" CACHE STRING "")
set(BUILD_OPENGL3_DEMOS "OFF" CACHE STRING "")
set(BUILD_UNIT_TESTS "OFF" CACHE STRING "")
set(OpenGL_GL_PREFERENCE "GLVND" CACHE STRING "")
set(BUILD_EXTRAS "OFF" CACHE STRING "")
set(BUILD_BULLET2_DEMOS "OFF" CACHE STRING "")
set(INSTALL_LIBS "OFF" CACHE STRING "")
add_subdirectory(bullet)

# GoogleTest
if(PACKAGE_TESTS)
  add_subdirectory(googletest)
endif()

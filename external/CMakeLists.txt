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

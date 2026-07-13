cmake_minimum_required(VERSION 3.10)

# ─────────────────────────────────────────
# Force the whole project onto GCC / MinGW-w64.
# Must be set BEFORE project() to take effect.
# (Also set your IDE's CMake "kit" to GCC/MinGW —
#  this alone won't override an IDE kit selection.)
# ─────────────────────────────────────────
if(NOT DEFINED CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER "C:/msys64/ucrt64/bin/gcc.exe")
endif()
if(NOT DEFINED CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe")
endif()
# ^ Hardcoded to the exact same compiler compile_project.bat uses.
# This matters: if CMake's kit resolves to a *different* MinGW install
# than the one in compile_project.bat, starisk_core.dll and your
# compiled project .dll end up linked against two different C++
# runtimes (libstdc++-6.dll / libgcc_s_seh-1.dll / libwinpthread-1.dll).
# That mismatch is a classic cause of "works standalone, crashes when
# the plugin DLL is loaded and called into."

project(starisk)
set(CMAKE_CXX_STANDARD 17)

# ─────────────────────────────────────────
# GLFW import library selection
# GCC/MinGW needs the MinGW-w64 build (libglfw3.a),
# NOT the MSVC-built glfw3.lib (causes corrupt .drectve
# warnings and can break linking of dynamically loaded DLLs).
# Download from glfw.org -> "lib-mingw-w64" and place at
# lib/mingw/libglfw3.a
# ─────────────────────────────────────────
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(GLFW_LIB ${CMAKE_SOURCE_DIR}/lib/mingw/libglfw3.a)
else()
    set(GLFW_LIB ${CMAKE_SOURCE_DIR}/lib/glfw3.lib)
endif()

# ─────────────────────────────────────────
# 1. Third-Party Static Library (ImGui & GLAD)
# ─────────────────────────────────────────
file(GLOB_RECURSE VENDOR_SOURCES 
    "src/core/imgui/*.cpp"
    "src/core/imgui/*.c"
    "src/core/*.c" # 👈 THIS catches glad.c if it lives inside src/core/
    "include/imgui/backends/imgui_impl_glfw.cpp"
    "include/imgui/backends/imgui_impl_opengl3.cpp"
)

add_library(starisk_vendor STATIC ${VENDOR_SOURCES})

target_include_directories(starisk_vendor PUBLIC
    include/
    include/imgui/
    include/imgui/backends/
)

target_link_libraries(starisk_vendor PUBLIC
    ${GLFW_LIB}
    opengl32
    gdi32
    user32
)

# ─────────────────────────────────────────
# 2. Core Engine Library (Shared DLL)
# ─────────────────────────────────────────
file(GLOB_RECURSE CORE_SOURCES 
    "src/core/*.cpp"
    "src/core/ECS/*.cpp"
)

# Filter out any stray imgui or glad files from the core cpp list
list(FILTER CORE_SOURCES EXCLUDE REGEX "imgui|glad")

add_library(starisk_core SHARED ${CORE_SOURCES})

target_include_directories(starisk_core PUBLIC
    include/
    include/imgui/
    include/imgui/backends/
)

# Link core to vendor so it can use GLAD symbols
target_link_libraries(starisk_core PUBLIC
    starisk_vendor
    ${GLFW_LIB}
    opengl32
    gdi32
    user32
)

target_precompile_headers(starisk_core PUBLIC
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/core/pch.h>"
)

# ─────────────────────────────────────────
# 3. Editor executable
# ─────────────────────────────────────────
file(GLOB_RECURSE EDITOR_SOURCES 
    "src/stariskEditor/*.cpp"
    "src/stariskEditor/*.c"
)

add_executable(starisk_editor ${EDITOR_SOURCES})

target_include_directories(starisk_editor PRIVATE
    include/
    include/imgui/
    include/imgui/backends/
    src/core/
)

target_link_libraries(starisk_editor PRIVATE 
    starisk_core 
    starisk_vendor
    ${GLFW_LIB}
    opengl32
    gdi32
    user32
)


# ─────────────────────────────────────────
# 4. Standalone Library Test
# ─────────────────────────────────────────
file(GLOB_RECURSE LIBTEST_SOURCES
    "src/projects/libTest/*.cpp"
)

add_executable(starisk_libtest ${LIBTEST_SOURCES})

target_include_directories(starisk_libtest PRIVATE
    include/
    include/imgui/
    include/imgui/backends/
    src/core/
)

target_link_libraries(starisk_libtest PRIVATE
    starisk_core
    starisk_vendor
    ${GLFW_LIB}
    opengl32
    gdi32
    user32
)
cmake_minimum_required(VERSION 3.10)
project(starisk)
set(CMAKE_CXX_STANDARD 17)

# ─────────────────────────────────────────
# 1. Core library — shared between editor and game projects
# ─────────────────────────────────────────
file(GLOB_RECURSE CORE_SOURCES 
    "src/core/*.cpp" 
    "src/core/*.c"
    "src/core/imgui/*.cpp"
    "src/core/imgui/*.c"
    "include/imgui/backends/imgui_impl_glfw.cpp"
    "include/imgui/backends/imgui_impl_opengl3.cpp"
)

add_library(starisk_core SHARED ${CORE_SOURCES})

target_include_directories(starisk_core PUBLIC
    include/
    include/imgui/
    include/imgui/backends/
)

target_link_libraries(starisk_core PUBLIC
    ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
    opengl32
    gdi32
    user32
)

target_precompile_headers(starisk_core PUBLIC
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/core/pch.h>"
)

# ─────────────────────────────────────────
# 2. Editor executable
# ─────────────────────────────────────────
file(GLOB_RECURSE EDITOR_SOURCES 
    "src/stariskEditor/*.cpp"
    "src/stariskEditor/*.c"
)

add_executable(starisk_editor ${EDITOR_SOURCES})

target_link_libraries(starisk_editor PRIVATE starisk_core)

target_include_directories(starisk_editor PRIVATE
    include/
    include/imgui/
    include/imgui/backends/
    src/core/
)


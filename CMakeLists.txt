cmake_minimum_required(VERSION 3.10)
project(starisk)
set(CMAKE_CXX_STANDARD 17)

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
    ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
    opengl32
    gdi32
    user32
)

# ─────────────────────────────────────────
# 2. Core Engine Library (Shared DLL)
# ─────────────────────────────────────────
file(GLOB_RECURSE CORE_SOURCES 
    "src/core/*.cpp"
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
    ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
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
    ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
    opengl32
    gdi32
    user32
)
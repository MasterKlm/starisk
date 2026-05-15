cmake_minimum_required(VERSION 3.10)


# set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
# set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")

project(starisk)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES "src/core/*.cpp" "src/stariskEditor/*.cpp" "src/stariskEditor/*.c" "src/core/imgui/*.cpp" "src/core/imgui/*.c" "src/core/*.c" "include/imgui/backends/imgui_impl_glfw.cpp"
"include/imgui/backends/imgui_impl_opengl3.cpp")

add_executable(starisk ${SOURCES})


target_include_directories(starisk PRIVATE
    include/
    include/imgui/
    include/imgui/backends/
)

target_link_libraries(starisk PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
    opengl32   # Windows
    gdi32      # Windows
    user32     # Windows
)

# Precompiled header
target_precompile_headers(starisk PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/core/pch.h>"
)
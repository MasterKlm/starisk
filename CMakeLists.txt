cmake_minimum_required(VERSION 3.10)


set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")

project(starisk)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")

add_executable(starisk ${SOURCES})


target_include_directories(starisk PRIVATE
    include/
    include/imgui/
    include/imgui/backends/
)

target_link_libraries(starisk PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
    opengl32   # Windows
    gdi32      # Windows
    user32     # Windows
)

# Precompiled header
target_precompile_headers(starisk PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>"
)
cmake_minimum_required(VERSION 3.16)  # target_precompile_headers needs 3.16+

# LLVM's official Windows installer (defaults to the MSVC target)
set(CMAKE_C_COMPILER   "C:/Program Files/LLVM/bin/clang.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")

project(starisk C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.c" "src/*.cpp")

add_executable(starisk ${SOURCES})

target_compile_definitions(starisk PRIVATE starisk_core_EXPORTS)

target_include_directories(starisk PRIVATE
    include/
    include/imgui
)

# glfw3.lib must be the MSVC-built static library (not the mingw .a)
target_link_libraries(starisk PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/msvc/glfw3.lib
    opengl32
    gdi32
    user32
    shell32
)

target_precompile_headers(starisk PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>"
)

# Optional: use LLVM's linker instead of MSVC link.exe
# target_link_options(starisk PRIVATE -fuse-ld=lld)

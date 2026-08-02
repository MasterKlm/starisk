cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER "C:/msys64/ucrt64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe")


project(starisk)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES "src/*.c" "src/*.cpp")


add_executable(starisk ${SOURCES})

target_compile_definitions(starisk PRIVATE starisk_core_EXPORTS)

target_include_directories(starisk PRIVATE
    include/
    include/imgui
)

target_link_libraries(starisk PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/mingw/libglfw3.a
    opengl32
    gdi32
    user32
)



target_precompile_headers(starisk PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h>"
)
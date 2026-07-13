@echo off
setlocal enabledelayedexpansion

:: Read from environment variables set by the editor
set PROJECT_FILE=%STARISK_SRC%
set OUTPUT_DLL=%STARISK_OUT%

echo [Batch] Compiling: %PROJECT_FILE%
echo [Batch] Outputting to: %OUTPUT_DLL%
echo [Batch] Using compiler: GCC (MinGW-w64)

:: Adjust this path if your MSYS2/MinGW install differs
set GXX=C:/msys64/ucrt64/bin/g++.exe

:: NOTE: -LC:/starisk/lib must point at a folder containing the
:: MinGW-w64 build of GLFW (libglfw3.a), matching lib/mingw in
:: CMakeLists.txt -- not the MSVC-built glfw3.lib.
set CMD="%GXX%" -shared -o "%OUTPUT_DLL%" "%PROJECT_FILE%" -g -O0 -D_DEBUG -Wno-attributes -IC:/starisk/include -IC:/starisk/include/imgui -IC:/starisk/src/core -LC:/starisk/build -lstarisk_core -lstarisk_vendor -LC:/starisk/lib/mingw -lglfw3 -lopengl32 -lgdi32 -luser32 -lshell32 -std=c++17

echo [Batch] Running: %CMD%
%CMD%

echo Compilation result: %ERRORLEVEL%
exit /b %ERRORLEVEL%
@echo off

:: Read from environment variables set by the editor
set PROJECT_FILE=%STARISK_SRC%
set OUTPUT_DLL=%STARISK_OUT%

echo [Batch] Compiling: %PROJECT_FILE%
echo [Batch] Outputting to: %OUTPUT_DLL%

clang++ -shared -o "%OUTPUT_DLL%" "%PROJECT_FILE%" ^
    -D_DEBUG ^
    -fms-runtime-lib=dll_dbg ^
    -Wno-nonportable-include-path ^
    -IC:/starisk/include ^
    -IC:/starisk/include/imgui ^
    -IC:/starisk/src/core ^
    -LC:/starisk/build ^
    -lstarisk_core ^
    -lstarisk_vendor ^
    -LC:/starisk/lib ^
    -lglfw3 ^
    -lopengl32 -lgdi32 -luser32 -lshell32 ^
    -std=c++17

echo Compilation result: %ERRORLEVEL%
exit /b %ERRORLEVEL%
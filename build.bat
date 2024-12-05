@echo off
echo Building litec...

if not exist "build" mkdir build
cd build

cmake -G "MinGW Makefiles" ..
if %ERRORLEVEL% neq 0 (
    echo CMake configuration failed!
    exit /b 1
)

cmake --build .
if %ERRORLEVEL% neq 0 (
    echo Build failed!
    exit /b 1
)

echo Build successful!

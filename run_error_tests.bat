@echo off
echo Running Lexer Error Test Cases
echo --------------------------------

set EXECUTABLE=.\build\litec.exe
set TEST_DIR=error_tests

if not exist "%EXECUTABLE%" (
    echo Error: Executable not found. Please build the project first.
    exit /b 1
)

for %%f in ("%TEST_DIR%\*.c") do (
    echo Testing: %%~nxf
    "%EXECUTABLE%" "%%f"
    echo.
)
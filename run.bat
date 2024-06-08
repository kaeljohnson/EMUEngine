@echo off
setlocal

:: Default to Debug if no configuration is specified
if "%~1"=="" (set CONFIG=Debug) else (set CONFIG=%~1)

:: Convert the configuration to lowercase
for /f %%i in ('echo %CONFIG% ^| powershell -Command "$input | ForEach-Object { $_.ToLower() }"') do set CONFIG=%%i

:: Check if the configuration is valid
if "%CONFIG%"=="distribution" (
    set CONFIG=release
) else if not "%CONFIG%"=="debug" if not "%CONFIG%"=="release" (
    echo Invalid configuration: %CONFIG%
    echo Please specify one of: debug, release, distribution
    exit /b 1
)

:: Check if vcpkg exists
if exist vcpkg\ (
    echo vcpkg exists
) else (
    cd Emu/external
    call git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    call bootstrap-vcpkg.bat
    call vcpkg integrate install
    call vcpkg install sdl2 sdl2-image
    call vcpkg install fmt
    cd ../../..
)

:: Build Box2D with the specified configuration
cd Emu\external\box2d\
echo Deleting wrong builds...
if "%CONFIG%"=="debug" if exist build\bin\release\ (rmdir /s /q build\bin\release)
if "%CONFIG%"=="release" if exist build\bin\debug\ (rmdir /s /q build\bin\debug)
if not exist build\bin\%CONFIG%\ (
    echo Building %CONFIG%...
    if not exist build\ (mkdir build)
    cd build
    cmake .. 
    cmake --build . --config %CONFIG%
    cd ..
)

cd ../../../

:: Run premake
call Emu\external\premake\premake5.exe vs2022

PAUSE
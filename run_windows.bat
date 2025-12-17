@echo off
setlocal

:: Default to Debug if no configuration is specified
if "%~1"=="" (set CONFIG=Debug) else (set CONFIG=%~1)

PAUSE

:: Convert to lowercase
for /f %%i in ('echo %CONFIG% ^| powershell -Command "$input | ForEach-Object { $_.ToLower() }"') do set CONFIG=%%i

PAUSE

:: Map "distribution" to "release"
if "%CONFIG%"=="distribution" (
    set CONFIG=release
) else if not "%CONFIG%"=="debug" if not "%CONFIG%"=="release" (
    echo Invalid configuration: %CONFIG%
    echo Please specify one of: debug, release, distribution
    PAUSE
    exit /b 1
)

:: Check if vcpkg exists
if not exist Emu\external\vcpkg\ (
    echo Cloning and bootstrapping vcpkg...
    cd Emu\external
    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    call bootstrap-vcpkg.bat
    call vcpkg integrate install
    cd ../../..
)

PAUSE

:: Install dependencies via vcpkg
cd Emu\external\vcpkg
call vcpkg install sdl2 sdl2-image sdl2-mixer gtest nlohmann-json
cd ../../..

PAUSE

:: Build Box2D with the specified configuration
cd Emu\external\box2d\
echo Deleting wrong builds...
if "%CONFIG%"=="debug" if exist build\bin\release\ (rmdir /s /q build\bin\release)
if "%CONFIG%"=="release" if exist build\bin\debug\ (rmdir /s /q build\bin\debug)
if not exist build\bin\%CONFIG%\ (
    echo Building %CONFIG%...
    if not exist build\ (mkdir build)
    cd build
    cmake -DBOX2D_BUILD_DOCS=ON -DCMAKE_INSTALL_PREFIX="install" ..
    cmake --build .
    cmake --build . --target INSTALL
    cd ..
)

cd ../../../

echo %CD%

PAUSE

:: Run premake
Emu\external\premake\premake5.exe vs2022

PAUSE
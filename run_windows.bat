@echo off
setlocal

:: Default to Debug if no configuration is specified
if "%~1"=="" (set CONFIG=Debug) else (set CONFIG=%~1)

:: Convert to lowercase
for /f %%i in ('echo %CONFIG% ^| powershell -Command "$input | ForEach-Object { $_.ToLower() }"') do set CONFIG=%%i

:: Map "distribution" to "release"
if "%CONFIG%"=="distribution" (
    set CONFIG=release
) else if not "%CONFIG%"=="debug" if not "%CONFIG%"=="release" (
    echo Invalid configuration: %CONFIG%
    echo Please specify one of: debug, release, distribution
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

:: Install dependencies via vcpkg
cd Emu\external\vcpkg
call vcpkg install sdl2 sdl2-image sdl2-mixer gtest nlohmann-json
cd ../../..

:: Build Box2D if needed
cd Emu\external\box2d
if not exist build\ (mkdir build)
cd build
if not exist install\ (
    echo Building Box2D (%CONFIG%)...
    cmake -DBOX2D_BUILD_DOCS=ON -DCMAKE_INSTALL_PREFIX="install" ..
    cmake --build . --config %CONFIG%
    cmake --install . --config %CONFIG%
)
cd ../../../..

:: Run premake
call Emu\external\premake\premake5.exe vs2022

PAUSE

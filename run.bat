if exist vcpkg\ (
    echo vcpkg exists
) else (
    cd v1/external
    call git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    call bootstrap-vcpkg.bat
    call vcpkg integrate install
    call vcpkg install sdl2 sdl2-image
    cd ../../..
)

cd v1\external\box2d\

if exist build\bin\ (
    echo Build folder exists. Check bin folder for binaries.
) else (
    call build.bat
)

cd ../../..

call v1\external\premake\premake5.exe vs2022
PAUSE
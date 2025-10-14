#!/bin/bash
set -e

CONFIG=${1:-Debug}
CONFIG=$(echo "$CONFIG" | tr '[:upper:]' '[:lower:]')

if [ "$CONFIG" == "distribution" ]; then
    CONFIG="release"
elif [ "$CONFIG" != "debug" ] && [ "$CONFIG" != "release" ]; then
    echo "Invalid configuration: $CONFIG"
    echo "Please specify one of: debug, release, distribution"
    exit 1
fi

# Ensure vcpkg exists
if [ ! -d "Emu/external/vcpkg" ]; then
    echo "Cloning vcpkg..."
    cd Emu/external
    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    ./vcpkg integrate install
    cd ../../..
fi

# Install dependencies
cd Emu/external/vcpkg
./vcpkg install sdl2 sdl2-image sdl2-mixer gtest nlohmann-json
cd ../../..

# Build Box2D
cd Emu/external/box2d
mkdir -p build
cd build
if [ ! -d "install" ]; then
    echo "Building Box2D ($CONFIG)..."
    cmake -DBOX2D_BUILD_DOCS=ON -DCMAKE_INSTALL_PREFIX="install" ..
    cmake --build . --config "$CONFIG"
    cmake --install . --config "$CONFIG"
fi
cd ../../../..

# Run premake
Emu/external/premake/premake5-osx --file=Emu/premake5.lua gmake2

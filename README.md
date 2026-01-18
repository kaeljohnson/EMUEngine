## License

This project is licensed under the MIT License. See the LICENSE file for details.

This project uses third-party libraries which are distributed under their own
licenses. See THIRD_PARTY_LICENSES.md for details.

## Overview

Below is the link to the doxygen documentation for EMU

[Documentation](https://kaeljohnson.github.io/EMUEngine/)

Emu is 2D game engine which has a backend written entirely in C++. The main use case of this engine 
is to build simple 2D tile based simulations. Clients can primarily interact with the engine via
a text file based tile map and a "rules" json file. The map and rules file format is detailed below. 
The engine api is not strict and can be extended as necessary. Clients should have an understanding of
C++ and JSON to effectively use the engine.

## Support
Currently, EMU is only supported on Windows.

## Getting Started

EMU is currently meant to be built as a static library. There are a few dependencies 
that are recommended to be used as shared libraries, which are built by vcpkg as such. Therefore, it
is necessary to either put those dll files somewhere visual studio can find them when running the exe.

The following steps are specifically for building EMU via the build_windows.bat script.

### Prerequisites
git, cmake, and Visual Studio.

### Buildiing EMU via build_windows.bat
1. Clone EMU repo from Github.
2. Run the build_windows.bat script with arguments "debug", "release", or "distribution". This script should build everything associated with the engine, including all dependencies, tests, and the SandBox.
3. Open EMUEngine.sln.
4. Make sure the build configuration in the visual studio solution matches what the build_windows.bat argument was.
5. Set Sandbox as startup project.
6. Run app via Visual Studio.
7. Can also run exe from SandBox folder in file explorer if all necessary binary files are there.

### Building EMU with build_windows_minimal.bat
This script builds only the engine and minimal necessary dependencies. The user will have to create their own exe and link to the engine manually.

## How to use

### Creating a tile map
Currently, the engine is meant to be used via a tilemap that is formatted as a text file. This file must follow these rules:
* Tiles depicted via numbers.
* Tiles separated by "|"
* There must be an equal number of tiles on each line in the file.
* There should be nothing else in the file

### Creating a rules file
The rules file is a json file that defines the characteristics that each tile has based on the tile id. Below is the detailed format and explanation:

    "Level Name":
    {
        "World": 
        {
            "NumLayers": <int> -> Number of layers to be rendered.
            "Physics":
            {
                "Gravity": [float, float] -> x, y gravity vector.
                "NumUnitsPerTile": <int> -> How many world units per tile are there?
            }
        },
        "Assets": 
        {
            "Sprites":
            {
                "PathToSpriteSheets": <string> -> Path to sprite sheets.
                "Textures":
                {
                    <key for this sprite sheet>: <png file name> -> name of spritesheet. 
                    have as many listed out here as necessary...
                }
            },
            "Audio":
            {
                "PathToAudioFiles": <string> -> Path to audio files.
                "Sounds":
                {
                    <wav file name>: <int> -> indicate which sound id this is.
                }
            }
        },
        "ComponentTemplates":
        {
            "Transforms": 
            {
                <Transform key name>:
                {
                    "ZIndex": <int> -> Layer that this entity belongs to.
                    "DrawDebug": <string> "red", "green", "black", "blue" to indicate what color the debug transform point should be.
                    "enabled": <bool> -> whether this transform is enabled at the start of the simulation.
                },
                as many transforms as necessary...
            },
            "Physics": 
            {
                <Physic object key name>:
                {
                    "SizeInUnits": [float, float] -> Size of the physics body in world units.
                    "BodyType": <string> -> "DYNAMIC", "STATIC", "SENSOR" See documentation to understand what each body type is.
                    "Category" : <string> -> Filter type: "NONE", "PLAYER", "MAP", "ALL"
                    "Mask" : <string> -> Filter type: "NONE", "PLAYER", "MAP", "ALL"
                    "GravityOn": <bool> -> is gravity on for this physics body?
                    "CheckSimpleContacts": <bool> -> Does the physics body do constant simple one way checks?
                    "enabled": <bool> -> is the physics body on simulation start?
                    "DrawDebug": <string> "red", "green", "black", "blue" to indicate what color the debug physics object should be.
                },
                as many physics object templates as necessary...
            },
            "Camera": 
            {
                <Camera key name>:
                {
                    "Window":
                    {
                        "X": <float> -> x position of camera in percent of screen.
                        "Y": <float> -> y position of camera in percent of screen.
                        "Width": <float> -> width of camera in percent of screen.
                        "Height": <float> -> height of camera in percent of screen.
                    },
                    "PixelsPerUnit": <int> -> how many pixels make up one world unit from this cameras view?
                    "ClampingOn": <bool> -> Does this camera clamp to the map borders?
                    "enabled": <bool> -> Is this camera enabled at start of simulation?
                },
                there can be as many cameras as a user wants...
            },
            "SpriteSheets": 
            {
                <Spritesheet key name>:
                {
                    "Path": <string> -> key for this sprite sheet.
                    "PixelsPerFrame": [int, int] -> how many pixels makeup this spritesheet.
                    "SizeInUnits": [float, float] -> how big is this sprite in world units?
                    "OffsetFromTransform": [float, float] -> offset in world units from the transform position.
                    "Height": <int> -> Height of the sprite sheet in frames.
                    "Width": <int> -> Width of the sprite sheet in frames.
                    "DrawDebug": <string> "red", "green", "black", "blue" to indicate what color the debug physics object should be.
                },
                as many spritesheets as needed...
            },
            "Animations": 
            {
                <Animations key name>:
                {
                    <Animation key name>: 
                    {
                        "id": <int> -> unique id for animation. Used so runtime does not have to query strings.
                        "Frames": [int, int, int, int] -> the frames that this animation makes up.
                        "FrameTime": <int> -> how many frames (based on 60hz tick rate) does each frame in animation last.
                        "Loop": <bool> -> does the animation loop?
                    }
                    add more animations as needed...
                },
                add more animations groups as needed...
            }
        },
        "CharacterRules": 
        {
            <tile id>: 
            { 
              "ActiveOnStart": <bool> -> is this entity active on start? 
              "Transform": <string> -> transform key.
              "Physics": <string> -> physics key.           
              "Camera": <string> -> camera key.                  
              "Animations": <string> -> animations key.                    
              "SpriteSheet": <string> -> spritesheet key.
            },
        }
    }


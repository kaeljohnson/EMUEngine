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
is necessary to put those dll files somewhere visual studio can find them when running the exe.

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

The following is a short summary of how the engine is intended to be used. Please peruse Sandbox along with the documentation to learn more.

### Main
Currently, the client owns the main function. There should be nothing special about it other than it needing to invoke the engine via:

* Engine::Init(<num entities needed>)

This function initializes the engine singleton, which can then be obtained via:

* Engine::EMU* engine = Engine::EMU::GetInstance();

### Getting a simulation running
Currently, running a simulation is as easy as calling the following functions before runtime.

The following creates a scene with the corresponding name:
* engine->Scenes_Create("Level1");

The following sets the gravity for simulation:
* engine->Scenes_SetGravity("Level1", Math2D::Point2D(0.0f, 100.0f));

The following adds the tile map and corresponding rules file to the scene.
engine->Scenes_AddTileMap("Level1", "TestMap2.txt", "TestSceneRules.json");

Lastly, the scene which is meant to be the first scene must be loaded before the app is started:
* engine->Scenes_Load("Level1");

Finally, run the app:
* engine->RunApp();

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

### Registering for Events
Currently, all code related to the runtime environment must be added via a c++ function. This includes registering callback functions for events.
Here is an example of adding a callback to a sensor event between two entities:

    engine->Scenes_RegisterContactCallback("Level1", Engine::BEGIN_SENSOR, 1, 2, [](const Engine::Contact event)
    {
		Engine::EMU::GetInstance()->Scenes_RegisterIOEventListener("Level1", Engine::W_KEY_DOWN, [](Engine::IOEvent& e)		
            {
                Engine::EMU::GetInstance()->Scenes_Load("Level2");
                e.Handled = true;
            });
    });

This function call adds an event listener for when there is a contact event between entities whose tile ID's are 1 and 2. When this event occurs the callback will run. This callback registers another event listener, this time an IO event listener which loads level 2 upon the W key being pressed. 

Events can be registered for at startup or during runtime.

### OnScenePlay/OnSceneEnd
Currently, the client can add items that need to be processed at the beginning and end of events. Here is an example of adding an OnScenePlay event:

    engine->Scenes_RegisterOnPlayEvent("Level1", []()
	{
		Engine::EMU::GetInstance()->PlaySound(1, 128, true);
	});

This call to Scenes_RegisterOnPlayEvent() plays the specified sound when Level 1 starts.

Similarly, adding an OnSceneEnd event:

    engine->Scenes_RegisterOnEndEvent("Level1", []()
	{
        Engine::EMU::GetInstance()->StopSound(1);
	});

### The Updater Components
Currently, specific runtime entity functionality is added via an updater component. Here is an example of adding an updater component to a player entity:

    Engine::EMU::GetInstance()->Scenes_AddComponent<Engine::PhysicsUpdater>("Level1", 1,
    [this](Engine::Entity entity) { Update(entity); });

This function call to Scenes_AddComponent() addes a PhysicsUpdater to the entity with the tile id of 1. This body of the callback is a client defined Update function which ostensibly updates the player movement each frame.

Some important considerations:
* EMU does not store types as strings to avoid complexity and unnecessary allocations. Therefore, it is up to the client to keep id and name associations in mind. For example, the id field in the animation component could be associated with an enum in the client.

#pragma once

#include "../../include/TileMap/TileMap.h"
#include "../../include/Logging/Logger.h"
#include "../../include/ECS/ECS.h"
#include "../../include/ECS/ComponentManager.h"
#include "../../include/Components.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

static json rulesJson; // Only one rules file per game for now so this will work.

// Max number of entities in the map allowed by engine.
static const int MAX_SIZE = 50000;

namespace Engine
{
	TileMap::TileMap(ECS& refECS, AssetManager& refAssetManager)
		: m_refECS(refECS), m_refAssetManager(refAssetManager), m_mapDimensions(0, 0)
	{
        m_tileMap.reserve(MAX_SIZE);
    }

    Entity TileMap::GetEntity(char tileChar) const
    {
		if (m_groupedEntitiesByCharMap.find(tileChar) != m_groupedEntitiesByCharMap.end() && !m_groupedEntitiesByCharMap.at(tileChar).empty())
		{
			return m_groupedEntitiesByCharMap.at(tileChar).front(); // Return the first entity with this character.
		}

        return m_refECS.INVALID_ENTITY;
    }

	const std::vector<Entity>& TileMap::GetEntities(char tileChar) const
	{
		if (m_groupedEntitiesByCharMap.find(tileChar) != m_groupedEntitiesByCharMap.end())
            return m_groupedEntitiesByCharMap.at(tileChar);

		return std::vector<Entity>(); // Return an empty vector if no entities found.
	}

    void TileMap::CreateMap(const std::string mapFile, const std::string rulesFile)
    {
        // Open and parse the rules file
        std::ifstream inFile(rulesFile);
        if (!inFile.is_open()) 
        {
            throw std::runtime_error("Failed to open rules file: " + rulesFile);
        }

        try 
        {
            inFile >> rulesJson;
        }
        catch (const json::parse_error& e) 
        {
            throw std::runtime_error("Failed to parse rules JSON: " + std::string(e.what()));
        }

        // Parse file.
        std::ifstream file(mapFile);
        if (!file)
        {
            ENGINE_CRITICAL("Failed to open map file: " + mapFile);
            return;
        }


		size_t numEntities = 0;

        std::string line;
        while (std::getline(file, line))
        {
            if (m_mapDimensions.Y == 0)
            {
                // Set the width to the length of the first line
                m_mapDimensions.X = (int)line.length();
            }
            else if (line.length() != m_mapDimensions.X)
            {
                // Handle error: line does not have the same length as the first line
                ENGINE_CRITICAL("Map is not a rectangle: line " + std::to_string(m_mapDimensions.Y + 1) + " has a different length");
				throw std::runtime_error("Map is not a rectangle: line " + std::to_string(m_mapDimensions.Y + 1) + " has a different length");
                return;
            }

			// Add each character to the map.
            int x = 0;
            for (char tileChar : line)
            {
				if (numEntities < MAX_SIZE) // Can't have more than MAX_SIZE entities.
                {
					if (tileChar == '-')
					{
						// Don't create an entity for empty tiles.
					}
					else
					{
                        
                        Entity tileEntity = m_refECS.CreateEntity();
						m_tileMap[{x, m_mapDimensions.Y}] = std::make_pair(tileEntity, tileChar);
                        m_groupedEntitiesByCharMap[tileChar].push_back(tileEntity);

						numEntities++; // Actual entity created.
					}
                }
                else
                {
                    ENGINE_CRITICAL("Map size exceeds maximum size of " + std::to_string(MAX_SIZE));
                }
				x++;
            }

            m_mapDimensions.Y++;
        }
	}

    Animation makeAnimation(const std::string& name, const json& jAnim, Vector2D<int> pixelsPerFrame, 
        const Vector2D<float> offsetFromTransform, const Vector2D<size_t> dimensions, const Vector2D<float> frameSize, const bool drawDebug, const std::string& debugColor) 
        // name, value, pixelsPerFrame, offsetFromTransform, dimensions, size, value
    {
        Animation a;
        a.m_name = name;
        a.m_frames = jAnim.at("Frames").get<std::vector<int>>();
		a.m_numFrames = a.m_frames.size();
        a.m_frameDuration = jAnim.at("FrameTime").get<int>();
		a.m_pixelsPerFrame = pixelsPerFrame;
		a.m_dimensions = dimensions;
		a.m_size = frameSize;
		a.m_offsetFromTransform = offsetFromTransform;
        a.m_loop = jAnim.at("Loop").get<bool>();
		a.m_drawDebug = drawDebug;
		a.m_debugColor = debugColor;

        return a;
    }

	void TileMap::LoadMap()
	{
        std::unordered_set<char> isSolid;
		size_t numUnitsPerTile = 0;

        // Read the NumMetersPerTile and store it in your member variable
        if (rulesJson.contains("NumMetersPerTile") && rulesJson["NumMetersPerTile"].is_number())
        {
            numUnitsPerTile = rulesJson["NumMetersPerTile"];
        }
        else
        {
            throw std::runtime_error("NumMetersPerTile not found or invalid in rules JSON");
        }

        auto& tileRules = rulesJson["Tile Rules"];

		// Check if the rules file contains a "SpriteSheetsPath" key
		if (!rulesJson.contains("PathToSpriteSheets"))
		{
			ENGINE_CRITICAL("No sprite sheets path found in rules file. Add path to folder with sprite pngs.");
			throw std::runtime_error("No sprite sheets path found in rules file. Add path to folder with sprite pngs.");
			return;
		}

		// Get the sprite sheets path.
		const auto& spriteSheetsPathJson = rulesJson["PathToSpriteSheets"];
		std::string spriteSheetsPath = spriteSheetsPathJson;
		ENGINE_INFO_D("Sprite sheets path: " + spriteSheetsPath);

        // Check if app has sprite folder.
		if (!std::filesystem::exists(spriteSheetsPath))
		{
			ENGINE_CRITICAL("Sprite sheets path does not exist: " + spriteSheetsPath);
			return;
		}


        // Create entity, character tiles.
        for (auto& [coords, info] : m_tileMap)
        {
            const char tileChar = info.second;
			const int x = coords.first;
            const int y = coords.second;

			// if (tileChar == '-') continue; // Skip empty tiles.
            
			Entity tileEntity = info.first;

            // Convert tile char to string key ("P", "S", etc.)
            std::string tileKey(1, tileChar);

            if (!tileRules.contains(tileKey))
            {
                ENGINE_INFO_D("No such tile exists: " + tileKey);
                continue;
            }

            json characterRulesJson = tileRules[tileKey];

            bool activeOnStart = true;
            if (characterRulesJson.contains("ActiveOnStart"))
            {
                const auto& activeOnStartJson = characterRulesJson["ActiveOnStart"];
                bool activeOnStartBool = activeOnStartJson;
                if (activeOnStartBool == false)
                {
                    activeOnStart = false;
                }
            }

		    const bool hasTransform = characterRulesJson.contains("Transform");
            if (hasTransform)
            {
                int zIndex = 0; // Default depth.
			    if (characterRulesJson["Transform"].contains("ZIndex"))
				{
					const auto& zIndexJson = characterRulesJson["Transform"]["ZIndex"];
					if (zIndexJson.is_number())
					{
						zIndex = zIndexJson.get<int>();
					}
					else if (zIndexJson.is_string())
					{
						// Handle string case if needed
						ENGINE_INFO_D("ZIndex is a string");
					}
				}

                bool drawDebug = false;
                std::string debugColor = "red";
                if (characterRulesJson["Transform"].contains("DrawDebug"))
                {
                    drawDebug = true;

                    const auto& drawDebugJson = characterRulesJson["Transform"]["DrawDebug"];
                    if (drawDebugJson.is_string())
                    {
                        debugColor = drawDebugJson;
                    }
                }

                m_refECS.AddComponent<Transform>(
                    tileEntity,
                    Vector2D<float>(x * static_cast<float>(numUnitsPerTile),
                        y * static_cast<float>(numUnitsPerTile)),
                    Vector2D<float>(static_cast<float>(numUnitsPerTile),
                        static_cast<float>(numUnitsPerTile)),
					1.0f, 1.0f, 1, zIndex, drawDebug, debugColor
                );
            }

            // check for camera.
		    const bool hasCamera = characterRulesJson.contains("Camera");
			if (hasCamera)
			{
                Vector2D<float> size = Vector2D<float>(0.0f, 0.0f);
				Vector2D<float> screenRatio = Vector2D<float>(1.0f, 1.0f);
				Vector2D<float> positionInFractionOfScreenSize = Vector2D<float>(0.0f, 0.0f);
			    int pixelsPerUnit = 0;
				bool clampingOn = false;

				if (characterRulesJson["Camera"].contains("Size"))
				{
					const auto& sizeJson = characterRulesJson["Camera"]["Size"];
					if (sizeJson.is_array() && sizeJson.size() == 2)
					{
						if (sizeJson[0].is_number() && sizeJson[1].is_number())
						{
							size.X = sizeJson[0].get<float>();
							size.Y = sizeJson[1].get<float>();
						}
					}
					else
					{
						ENGINE_ERROR_D("Camera size is not an array of two numbers");
					}
				}

                if (characterRulesJson["Camera"].contains("PixelsPerUnit"))
                {
					const auto& pixelsPerUnitJson = characterRulesJson["Camera"]["PixelsPerUnit"];
                    if (pixelsPerUnitJson.is_number())
                    {
						pixelsPerUnit = pixelsPerUnitJson.get<int>();
                    }
					else
					{
						ENGINE_ERROR_D("PixelsPerUnit is not a number");
					}
                }

			    if (characterRulesJson["Camera"].contains("ClampingOn"))
				{
					const auto& clampingOnJson = characterRulesJson["Camera"]["ClampingOn"];
					if (clampingOnJson.is_boolean())
					{
						clampingOn = clampingOnJson.get<bool>();
					}
					else
					{
						ENGINE_ERROR_D("ClampingOn is not a boolean");
					}
				}
				if (characterRulesJson["Camera"].contains("Window"))
				{
					const auto& windowJson = characterRulesJson["Camera"]["Window"];
                    if (windowJson.contains("X") && windowJson.contains("Y"))
                    {
						if (windowJson["X"].is_number() && windowJson["Y"].is_number())
						{
							positionInFractionOfScreenSize.X = windowJson["X"].get<float>();
							positionInFractionOfScreenSize.Y = windowJson["Y"].get<float>();
						}
					}
					else
					{
						ENGINE_ERROR_D("Window is missing X or Y");
                    }

                    if (windowJson.contains("Width") && windowJson.contains("Height"))
                    {
                        if (windowJson["Width"].is_number() && windowJson["Height"].is_number())
                        {
                            screenRatio.X = windowJson["Width"].get<float>();
                            screenRatio.Y = windowJson["Height"].get<float>();
                        }

                    }
				}

				m_refECS.AddComponent<Camera>(tileEntity, size, screenRatio, positionInFractionOfScreenSize, pixelsPerUnit, clampingOn);
			}

            // Add Physics components.
            BodyType bodyType = STATIC;
            Filter category = ALL;
            Filter mask = ALL;
            Vector2D<float> size = Vector2D<float>(static_cast<float>(numUnitsPerTile), static_cast<float>(numUnitsPerTile));
            bool gravityOn = false;
            bool checkSimpleContacts = false;
            bool useChains = false;

            if (characterRulesJson.contains("Physics"))
            {
                json characterPhysicsRulesJson = characterRulesJson["Physics"];

                // Physics library needs to know if the body is enabled or not.
                const bool enabled = activeOnStart;

                if (characterPhysicsRulesJson.contains("Category"))
                {
                    const auto& categoryJson = characterPhysicsRulesJson["Category"];
                    std::string categoryStr = categoryJson;
                    if (categoryStr == "NONE") category = NONE;
                    else if (categoryStr == "PLAYER") category = PLAYER;
                    else if (categoryStr == "MAP") category = MAP;
                    else if (categoryStr == "ALL") category = ALL;
                }
                if (characterPhysicsRulesJson.contains("Mask"))
                {
                    const auto& maskJson = characterPhysicsRulesJson["Mask"];
                    std::string maskStr = maskJson;
                    if (maskStr == "NONE") mask = NONE;
                    else if (maskStr == "PLAYER") mask = PLAYER;
                    else if (maskStr == "MAP") mask = MAP;
                    else if (maskStr == "ALL") mask = ALL;
                }
                if (characterPhysicsRulesJson.contains("BodyType"))
                {
                    const auto& bodyTypeJson = characterPhysicsRulesJson["BodyType"];
                    std::string bodyTypeStr = bodyTypeJson;
                    if (bodyTypeStr == "STATIC") bodyType = STATIC;
                    else if (bodyTypeStr == "DYNAMIC") bodyType = DYNAMIC;
                    else if (bodyTypeStr == "KINEMATIC") bodyType = KINEMATIC;
                    else if (bodyTypeStr == "SENSOR") bodyType = SENSOR;
                }

                if (bodyType != SENSOR)
                {
					// If body is not a sensor, it is solid and collides with other bodies.
                    // This information is needed for the chain system to know which tiles to link to.
                    // Not the best design but will work for now.
                    isSolid.emplace(tileChar);

                    if (characterPhysicsRulesJson.contains("UseChains")) // Chains are not used for sensors.
                    {
                        const auto& useChainsJson = characterPhysicsRulesJson["UseChains"];
                        bool useChainsStr = useChainsJson;
                        if (useChainsStr == true)
                        {
                            useChains = true;
                        }
                    }
                }

                if (characterPhysicsRulesJson.contains("GravityOn"))
                {
                    const auto& gravityOnJson = characterPhysicsRulesJson["GravityOn"];
                    bool gravityOnStr = gravityOnJson;
                    if (gravityOnStr == true)
                    {
                        gravityOn = true;
                    }
                }
                if (characterPhysicsRulesJson.contains("CheckSimpleContacts"))
                {
                    const auto& checkSimpleContactsJson = characterPhysicsRulesJson["CheckSimpleContacts"];
                    bool checkSimpleContactsStr = checkSimpleContactsJson;
                    if (checkSimpleContactsStr == true)
                    {
                        checkSimpleContacts = true;
                    }

                }
                if (characterPhysicsRulesJson.contains("DefaultPhysicsBodySize"))
                {
                    const auto& sizeJson = characterPhysicsRulesJson["DefaultPhysicsBodySize"];
                    if (sizeJson.is_array() && sizeJson.size() == 2)
                    {
                        if (sizeJson[0].is_number() && sizeJson[1].is_number())
                        {
                            size.X = sizeJson[0].get<float>();
                            size.Y = sizeJson[1].get<float>();
                        }
                    }
                }

                if (useChains) // Chains are used on tiles, typically map tiles, to avoid ghose collisions on adjacent tiles.
                {
                    auto isTileSolid = [&](int x, int y) -> bool 
                    {
                        const auto* tile = GetTile(x, y);
                        return tile && isSolid.find(tile->second) != isSolid.end();
                    };

                    bool hasTileAbove = y > 0 && isTileSolid(x, y - 1);
                    bool hasTileBelow = y < GetHeight() - 1 && isTileSolid(x, y + 1);
                    bool hasTileLeft = x > 0 && GetTile(x - 1, y) && isTileSolid(x - 1, y);
                    bool hasTileRight = x < GetWidth() - 1 && isTileSolid(x + 1, y);

                    bool hasTileDiagonalLeftAbove = x > 0 && y > 0 && isTileSolid(x - 1, y - 1);
                    bool hasTileDiagonalLeftBelow = x > 0 && y < GetHeight() - 1 && isTileSolid(x - 1, y + 1);
                    bool hasTileDiagonalRightAbove = x < GetWidth() - 1 && y > 0 && isTileSolid(x + 1, y - 1);
                    bool hasTileDiagonalRightBelow = x < GetWidth() - 1 && y < GetHeight() - 1 && isTileSolid(x + 1, y + 1);

                    if (!hasTileAbove)
                    {
                        float ghostX0, ghostY0;
                        float x1 = (float)x;
                        float y1 = (float)y;
                        float x2 = x + 1.0f;
                        float y2 = (float)y;
                        float ghostX3, ghostY3;

                        if (!hasTileRight) { ghostX3 = (float)x + 1.0f; ghostY3 = (float)y + 1.0f; }
                        else if (hasTileRight && hasTileDiagonalRightAbove) { ghostX3 = (float)x + 1.0f; ghostY3 = (float)y - 1.0f; }
                        else if (hasTileRight) { ghostX3 = (float)x + 2.0f; ghostY3 = (float)y; }

                        if (!hasTileLeft) { ghostX0 = (float)x; ghostY0 = (float)y + 1.0f; }
                        else if (hasTileLeft && hasTileDiagonalLeftAbove) { ghostX0 = (float)x; ghostY0 = (float)y - 1.0f; }
                        else if (hasTileLeft) { ghostX0 = (float)x - 1.0f; ghostY0 = (float)y; }

                        m_refECS.AddComponent<ChainColliderTop>(tileEntity, enabled, category, mask, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
                            Vector2D<float>(x2, y2), Vector2D<float>(ghostX3, ghostY3));
                    }

                    if (!hasTileLeft)
                    {
                        float ghostX0, ghostY0;
                        float x1 = (float)x;
                        float y1 = y + 1.0f;
                        float x2 = (float)x;
                        float y2 = (float)y;
                        float ghostX3, ghostY3;

                        if (!hasTileAbove) { ghostX3 = x + 1.0f; ghostY3 = (float)y; }
                        else if (hasTileAbove && hasTileDiagonalLeftAbove) { ghostX3 = (float)x - 1.0f; ghostY3 = (float)y; }
                        else if (hasTileAbove) { ghostX3 = (float)x; ghostY3 = (float)y - 1.0f; }

                        if (!hasTileBelow) { ghostX0 = (float)x + 1.0f; ghostY0 = (float)y + 1.0f; }
                        else if (hasTileBelow && hasTileDiagonalLeftBelow) { ghostX0 = (float)x - 1.0f; ghostY0 = (float)y + 1.0f; }
                        else if (hasTileBelow) { ghostX0 = (float)x; ghostY0 = (float)y + 2.0f; }

                        m_refECS.AddComponent<ChainColliderLeft>(tileEntity, enabled, category, mask, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
                            Vector2D<float>(x2, y2), Vector2D<float>(ghostX3, ghostY3));
                    }

                    if (!hasTileBelow)
                    {
                        float ghostX0, ghostY0;
                        float x1 = x + 1.0f;
                        float y1 = y + 1.0f;
                        float x2 = (float)x;
                        float y2 = y + 1.0f;
                        float ghostX3, ghostY3;

                        if (!hasTileLeft) { ghostX3 = (float)x; ghostY3 = (float)y; }
                        else if (hasTileLeft && hasTileDiagonalLeftBelow) { ghostX3 = (float)x; ghostY3 = (float)y + 2.0f; }
                        else if (hasTileLeft) { ghostX3 = (float)x - 1.0f; ghostY3 = (float)y + 1.0f; }

                        if (!hasTileRight) { ghostX0 = (float)x + 1.0f; ghostY0 = (float)y; }
                        else if (hasTileRight && hasTileDiagonalRightBelow) { ghostX0 = (float)x + 1.0f; ghostY0 = (float)y + 2.0f; }
                        else if (hasTileRight) { ghostX0 = (float)x + 2.0f; ghostY0 = (float)y + 1.0f; }

                        m_refECS.AddComponent<ChainColliderBottom>(tileEntity, enabled, category, mask, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
                            Vector2D<float>(x2, y2), Vector2D<float>(ghostX3, ghostY3));
                    }

                    if (!hasTileRight)
                    {
                        float ghostX0, ghostY0;
                        float x1 = x + 1.0f;
                        float y1 = (float)y;
                        float x2 = x + 1.0f;
                        float y2 = y + 1.0f;
                        float ghostX3, ghostY3;

                        if (!hasTileAbove) { ghostX0 = (float)x; ghostY0 = (float)y; }
                        else if (hasTileAbove && hasTileDiagonalRightAbove) { ghostX0 = (float)x + 2.0f; ghostY0 = (float)y; }
                        else if (hasTileAbove) { ghostX0 = (float)x + 1.0f; ghostY0 = (float)y - 1.0f; }

                        if (!hasTileBelow) { ghostX3 = (float)x; ghostY3 = (float)y + 1.0f; }
                        else if (hasTileBelow && hasTileDiagonalRightBelow) { ghostX3 = (float)x + 2.0f; ghostY3 = (float)y + 1.0f; }
                        else if (hasTileBelow) { ghostX3 = (float)x + 1.0f; ghostY3 = (float)y + 2.0f; }

                        m_refECS.AddComponent<ChainColliderRight>(tileEntity, enabled, category, mask, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
                            Vector2D<float>(x2, y2), Vector2D<float>(ghostX3, ghostY3));
                    }
                }
                else // Create a regular physics body for the tiles that don't use chains.
                {
                    m_refECS.AddComponent<PhysicsBody>(tileEntity, enabled, bodyType, category, mask,
                        Vector2D<float>(size.X, size.Y),
                        Vector2D<float>(static_cast<float>(x) * static_cast<float>(numUnitsPerTile), static_cast<float>(y) * static_cast<float>(numUnitsPerTile)),
                        0.0f, gravityOn, checkSimpleContacts);
                }
            }

			// Add sprite component.
			if (characterRulesJson.contains("SpriteSheet") && characterRulesJson.contains("Animations"))
			{
				const auto& spriteSheetJson = characterRulesJson["SpriteSheet"];
				const auto& animationsJson = characterRulesJson["Animations"];

                std::string spriteSheetPath;
                if (!spriteSheetJson.contains("Path"))
                {
                    ENGINE_CRITICAL("No path for sprite sheet found.");
					spriteSheetPath = "default.png"; // Default sprite sheet path.
                    continue;
                }

				spriteSheetPath = spriteSheetJson["Path"];

				// ENGINE_INFO_D("Loading texture from: " + spriteSheetPath);
                // The following must only be called once per scene.
                // Currently, it is called for every character that has a animation or sprite from
                // the texture. That takes way too long and wastes resources!!!
				m_refAssetManager.LoadTexture(tileEntity, spriteSheetsPath + spriteSheetPath); // Should this be called on scene play?



                Vector2D<float> frameSize = { 1.0f, 1.0f }; // size of sprite in units.
                if (!spriteSheetJson.contains("SizeInUnits"))
                {
                    ENGINE_CRITICAL("No size for sprite sheet found. Defaulting to 1, 1");
                }
				else
				{
					const auto& sizeJson = spriteSheetJson["SizeInUnits"];
					if (sizeJson.is_array() && sizeJson.size() == 2)
					{
						// ENGINE_INFO_D("Found size for sprite sheet");
						if (sizeJson[0].is_number() && sizeJson[1].is_number())
						{
							frameSize.X = sizeJson[0].get<float>();
							frameSize.Y = sizeJson[1].get<float>();
						}
					}
				}

				Vector2D<int> pixelsPerFrame = { 32, 32 }; // size of sprite frame in pixels.
				if (!spriteSheetJson.contains("PixelsPerFrame"))
				{
					ENGINE_CRITICAL("No size for sprite sheet found. Defaulting to 32, 32");
				}
				else
				{
					const auto& pixelsPerFrameJson = spriteSheetJson["PixelsPerFrame"];
					if (pixelsPerFrameJson.is_array() && pixelsPerFrameJson.size() == 2)
					{
						if (pixelsPerFrameJson[0].is_number() && pixelsPerFrameJson[1].is_number())
						{
							pixelsPerFrame.X = pixelsPerFrameJson[0].get<int>();
							pixelsPerFrame.Y = pixelsPerFrameJson[1].get<int>();
						}
					}
				}

				Vector2D<float> offsetFromTransform = { 0.0f, 0.0f };
                if (!spriteSheetJson.contains("OffsetFromTransform"))
                {
					ENGINE_INFO_D("No offset for sprite sheet found. Defaulting to 0, 0");
                }
                else
				{
					const auto& offsetJson = spriteSheetJson["OffsetFromTransform"];
					if (offsetJson.is_array() && offsetJson.size() == 2)
					{
						if (offsetJson[0].is_number() && offsetJson[1].is_number())
						{
							offsetFromTransform.X = offsetJson[0].get<float>();
							offsetFromTransform.Y = offsetJson[1].get<float>();
						}
					}
				}

				Vector2D<size_t> dimensions = { 1, 1 }; // Default dimensions.
				if (spriteSheetJson.contains("Width") && spriteSheetJson.contains("Height"))
				{
					const auto& widthJson = spriteSheetJson["Width"];
					const auto& heightJson = spriteSheetJson["Height"];
					if (widthJson.is_number() && heightJson.is_number())
					{
						dimensions.X = widthJson.get<size_t>();
						dimensions.Y = heightJson.get<size_t>();
					}
				}

                bool drawDebug = false;
				std::string debugColor = "red";
				if (characterRulesJson.contains("SpriteSheet") && characterRulesJson["SpriteSheet"].contains("DrawDebug"))
				{
					drawDebug = true;

					const auto& drawDebugJson = characterRulesJson["SpriteSheet"]["DrawDebug"];
					if (drawDebugJson.is_string())
					{
                        debugColor = drawDebugJson;

					}
				}

				// TODO: Get animation data from the JSON file.
				std::unordered_map<std::string, Animation> animations;
				json j = json::parse(characterRulesJson["Animations"].dump());

                for (auto& [name, value] : j.items())
                {
                    animations.emplace(name, makeAnimation(name, value, pixelsPerFrame, offsetFromTransform,
                        dimensions, frameSize, drawDebug, debugColor));
                }


				m_refECS.AddComponent<Animations>(tileEntity, animations);
			}

            if (characterRulesJson.contains("AudioSource"))
            {
				const auto& audioJson = characterRulesJson["Audio"];
				if (audioJson.contains("Path"))
				{
					std::string audioPath = audioJson["Path"];
					m_refECS.AddComponent<AudioSource>(tileEntity, audioPath);
				}
            }

            // If the character is "ActiveOnStart", activate it in the ECS.
            // MUST CALL AFTER ALL COMPONENTS CREATED.
            if (activeOnStart)
            {
                m_refECS.Activate(tileEntity);
            }
        }

	}

    void TileMap::UnloadMap()
	{
        // Nothing to do. Scene destroys components.
	}

    const std::pair<Entity, char>* TileMap::GetTile(int x, int y) const
    {
		std::pair<int, int> key = std::make_pair(x, y);
        auto it = m_tileMap.find(key);
        if (it != m_tileMap.end())
            return &it->second; // return pointer to value

        return nullptr;
    }
}
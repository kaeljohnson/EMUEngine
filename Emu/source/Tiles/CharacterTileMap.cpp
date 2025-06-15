#pragma once

#include "../../include/CharacterTileMap/CharacterTileMap.h"
#include "../../include/Logging/Logger.h"
#include "../../include/ECS/ECS.h"
#include "../../include/ECS/ComponentManager.h"
#include "../../include/Components.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

static json rulesJson; // Only one rules file per game for now so this will work.

namespace Engine
{
	CharacterTileMap::CharacterTileMap(ECS& refECS, AssetManager& refAssetManager)
		: m_refECS(refECS), m_refAssetManager(refAssetManager), m_mapDimensions(0, 0), m_numUnitsPerTile(0)
	{
		m_tiles.reserve(MAX_SIZE);
		m_allMapEntities.reserve(MAX_SIZE);
    }

    Entity CharacterTileMap::GetEntity(char tileChar) const
    {
		if (m_sameCharEntitiesMap.find(tileChar) != m_sameCharEntitiesMap.end() && !m_sameCharEntitiesMap.at(tileChar).empty())
		{
			return m_sameCharEntitiesMap.at(tileChar).front(); // Return the first entity with this character.
		}

        return m_refECS.INVALID_ENTITY;
    }

	const std::vector<Entity>& CharacterTileMap::GetEntities(char tileChar) const
	{
		if (m_sameCharEntitiesMap.find(tileChar) != m_sameCharEntitiesMap.end())
            return m_sameCharEntitiesMap.at(tileChar);

		return std::vector<Entity>(); // Return an empty vector if no entities found.
	}

    void CharacterTileMap::CreateMap(const std::string mapFile, const std::string rulesFile)
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

        // Read the NumMetersPerTile and store it in your member variable
        if (rulesJson.contains("NumMetersPerTile") && rulesJson["NumMetersPerTile"].is_number())
        {
            m_numUnitsPerTile = rulesJson["NumMetersPerTile"];
        }
        else
        {
            throw std::runtime_error("NumMetersPerTile not found or invalid in rules JSON");
        }

        // Parse file.
        std::ifstream file(mapFile);
        if (!file)
        {
            ENGINE_CRITICAL("Failed to open map file: " + mapFile);
            return;
        }


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
                return;
            }

			// Add each character to the map.
            int x = 0;
            for (char tileChar : line)
            {
				if (m_allMapEntities.size() < MAX_SIZE) // Can't have more than MAX_SIZE entities.
                {
					if (tileChar == '-')
					{
                        m_tiles.push_back(std::make_tuple(m_refECS.INVALID_ENTITY, tileChar, Vector2D<int>(x, m_mapDimensions.Y)));
					}
					else
					{
                        Entity tileEntity = m_refECS.CreateEntity();
                        m_tiles.push_back(std::make_tuple(tileEntity, tileChar, Vector2D<int>(x, m_mapDimensions.Y)));
                        m_allMapEntities.push_back(std::make_pair(tileEntity, tileChar));
						m_sameCharEntitiesMap[tileChar].push_back(tileEntity);
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

	void CharacterTileMap::LoadMap()
	{
        // Read the NumMetersPerTile and store it in your member variable
        if (rulesJson.contains("NumMetersPerTile") && rulesJson["NumMetersPerTile"].is_number())
        {
            m_numUnitsPerTile = rulesJson["NumMetersPerTile"];
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
            // Need to crash app here.
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
        for (auto& tuple : m_tiles)
        {
		    const char tileChar = std::get<1>(tuple);
			const int x = std::get<2>(tuple).X;
			const int y = std::get<2>(tuple).Y;

			if (tileChar == '-') continue; // Skip empty tiles.
            
			Entity tileEntity = std::get<0>(tuple);

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

                m_refECS.AddComponent<Transform>(
                    tileEntity,
                    Vector2D<float>(x * static_cast<float>(m_numUnitsPerTile),
                        y * static_cast<float>(m_numUnitsPerTile)),
                    Vector2D<float>(static_cast<float>(m_numUnitsPerTile),
                        static_cast<float>(m_numUnitsPerTile)),
                    1.0f, 1.0f, 1, zIndex
                );
            }

            // check for camera.
		    const bool hasCamera = characterRulesJson.contains("Camera");
			if (hasCamera)
			{
                Vector2D<float> size = Vector2D<float>(0.0f, 0.0f);
				Vector2D<float> screenRatio = Vector2D<float>(1.0f, 1.0f);
				Vector2D<float> position = Vector2D<float>(0.0f, 0.0f);
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
							position.X = windowJson["X"].get<float>();
							position.Y = windowJson["Y"].get<float>();
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

				m_refECS.AddComponent<Camera>(tileEntity, size, screenRatio, position, pixelsPerUnit, clampingOn);
			}

            // Add Physics components.
            BodyType bodyType = STATIC;
            Filter category = ALL;
            Filter mask = ALL;
            Vector2D<float> size = Vector2D<float>(static_cast<float>(m_numUnitsPerTile), static_cast<float>(m_numUnitsPerTile));
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
				if (characterPhysicsRulesJson.contains("UseChains") && bodyType != SENSOR) // Chains are not used for sensors.
                {
                    const auto& useChainsJson = characterPhysicsRulesJson["UseChains"];
                    bool useChainsStr = useChainsJson;
                    if (useChainsStr == true)
                    {
                        useChains = true;
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
                    // Can't use 'w' here. Need to instead determine if the tile being checked for is a sensor or has collisions.
                    bool hasTileAbove = (y > 0 && std::get<1>(GetTile(x, y - 1)) == 'w');
                    bool hasTileBelow = (y < GetHeight() - 1 && std::get<1>(GetTile(x, y + 1)) == 'w');
                    bool hasTileLeft = (x > 0 && std::get<1>(GetTile(x - 1, y)) == 'w');
                    bool hasTileRight = (x < GetWidth() - 1 && std::get<1>(GetTile(x + 1, y)) == 'w');

                    bool hasTileDiagonalLeftAbove = (x > 0 && y > 0 && std::get<1>(GetTile(x - 1, y - 1)) == 'w');
                    bool hasTileDiagonalLeftBelow = (x > 0 && y < GetHeight() - 1 && std::get<1>(GetTile(x - 1, y + 1)) == 'w');
                    bool hasTileDiagonalRightAbove = (x < GetWidth() - 1 && y > 0 && std::get<1>(GetTile(x + 1, y - 1)) == 'w');
                    bool hasTileDiagonalRightBelow = (x < GetWidth() - 1 && y < GetHeight() - 1 && std::get<1>(GetTile(x + 1, y + 1)) == 'w');

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
                        Vector2D<float>(static_cast<float>(x) * static_cast<float>(m_numUnitsPerTile), static_cast<float>(y) * static_cast<float>(m_numUnitsPerTile)),
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

				ENGINE_INFO_D("Loading texture from: " + spriteSheetPath);
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
						ENGINE_INFO_D("Found size for sprite sheet");
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

				// TODO: Get animation data from the JSON file.

				m_refECS.AddComponent<Sprite>(tileEntity, spriteSheetPath, frameSize, pixelsPerFrame, offsetFromTransform, 0, 100);
			}

            // If the character is "ActiveOnStart", activate it in the ECS.
            // MUST CALL AFTER ALL COMPONENTS CREATED.
            if (activeOnStart)
            {
                m_refECS.Activate(tileEntity);
            }
        }

	}

    void CharacterTileMap::UnloadMap()
	{
        // Nothing to do. Scene destroys components.
	}

    const char CharacterTileMap::GetChar(size_t x, size_t y) const
    {
        if (x < 0 || x >= m_mapDimensions.X || y < 0 || y >= m_mapDimensions.Y)
        {
            return ' ';
        }

        return std::get<1>(m_tiles[y * m_mapDimensions.X + x]);
    }

    const std::tuple<Entity, char, Vector2D<int>> CharacterTileMap::GetTile(size_t x, size_t y) const
    {
        // This needs to return the associated entity as well.
		if (x < 0 || x >= m_mapDimensions.X || y < 0 || y >= m_mapDimensions.Y) 
		{
			return std::make_tuple(m_refECS.INVALID_ENTITY, ' ', Vector2D<int>((int)x, (int)y));
		}
		return m_tiles[y * m_mapDimensions.X + x];
	}
}
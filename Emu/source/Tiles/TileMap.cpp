#pragma once

#include "../../include/Tiles/TileMap.h"
#include "../../include/Logging/Logger.h"
#include "../../include/ECS/ECS.h"
#include "../../include/ECS/ComponentManager.h"
#include "../../include/Components.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

static json rulesJson; // Only one rules file per game for now so this will work.

namespace Engine
{
	TileMap::TileMap(ECS& refECS)
		: m_refECS(refECS), m_mapDimensions(0, 0), m_numUnitsPerTile(0)
	{
		m_map.reserve(MAX_SIZE);
		m_tiles.reserve(MAX_SIZE);
		m_allMapEntities.reserve(MAX_SIZE);
    }

    Entity TileMap::GetEntity(char tileChar) const
    {
        for (const auto& tile : m_allMapEntities)
        {
            if (tile.second == tileChar)
            {
                return tile.first;
            }
        }
        return m_refECS.INVALID_ENTITY;
    }

	std::vector<Entity> TileMap::GetEntities(char tileChar) const
	{
		std::vector<Entity> entities;
		for (const auto& tile : m_allMapEntities)
		{
			if (tile.second == tileChar)
			{
				entities.push_back(tile.first);
			}
		}
		return entities;
	}

    void TileMap::CreateMap(const std::string mapFile, const std::string rulesFile)
    {
        m_map.reserve(MAX_SIZE);
        m_tiles.reserve(MAX_SIZE);

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

            for (char c : line)
            {
                if (m_map.size() < MAX_SIZE)
                {
                    m_map.push_back(c);
                }
                else
                {
                    ENGINE_CRITICAL("Map size exceeds maximum size of " + std::to_string(MAX_SIZE));
                }
            }

            m_mapDimensions.Y++;
        }

        // Create entity, character tiles.
        for (int y = 0; y < GetHeight(); ++y)
        {
            for (int x = 0; x < GetWidth(); ++x)
            {
                const char tileChar = GetChar(x, y);

                if (tileChar != '-')
                {
                    Entity tileEntity = m_refECS.CreateEntity();

                    m_tiles.push_back(std::make_tuple(tileEntity, tileChar, Vector2D<int>(x, y)));
					m_allMapEntities.push_back(std::make_pair(tileEntity, tileChar));
                }
                else
                {
                    m_tiles.push_back(std::make_tuple(m_refECS.INVALID_ENTITY, tileChar, Vector2D<int>(x, y)));
                }
            }
        }
	}

	void TileMap::LoadMap()
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

		    const bool hasTransform = tileRules[tileKey].contains("Transform");
            if (hasTransform)
            {
                int zIndex = 0; // Default depth.
			    if (tileRules[tileKey]["Transform"].contains("ZIndex"))
				{
					const auto& zIndexJson = tileRules[tileKey]["Transform"]["ZIndex"];
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
		    const bool hasCamera = tileRules[tileKey].contains("Camera");
			if (hasCamera)
			{
                Vector2D<float> size = Vector2D<float>(0.0f, 0.0f);
			    int pixelsPerUnit = 0;
				bool clampingOn = false;

				if (tileRules[tileKey]["Camera"].contains("Size"))
				{
					const auto& sizeJson = tileRules[tileKey]["Camera"]["Size"];
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

                if (tileRules[tileKey]["Camera"].contains("PixelsPerUnit"))
                {
					const auto& pixelsPerUnitJson = tileRules[tileKey]["Camera"]["PixelsPerUnit"];
                    if (pixelsPerUnitJson.is_number())
                    {
						pixelsPerUnit = pixelsPerUnitJson.get<int>();
                    }
					else
					{
						ENGINE_ERROR_D("PixelsPerUnit is not a number");
					}
                }

			    if (tileRules[tileKey]["Camera"].contains("ClampingOn"))
				{
					const auto& clampingOnJson = tileRules[tileKey]["Camera"]["ClampingOn"];
					if (clampingOnJson.is_boolean())
					{
						clampingOn = clampingOnJson.get<bool>();
					}
					else
					{
						ENGINE_ERROR_D("ClampingOn is not a boolean");
					}
				}

				m_refECS.AddComponent<Camera>(tileEntity, size, pixelsPerUnit, clampingOn);
			}
        }


        // Create collision bodies for the tiles.
        CreateCollisionBodies();
	}

    void TileMap::CreateCollisionBodies()
    {
        auto& tileRules = rulesJson["Tile Rules"];

        for (auto& tuple : m_tiles)
        {
            BodyType bodyType = STATIC;
            Filter category = ALL;
            Filter mask = ALL;
		    Vector2D<float> size = Vector2D<float>(static_cast<float>(m_numUnitsPerTile), static_cast<float>(m_numUnitsPerTile));
			bool gravityOn = false;
			bool checkSimpleContacts = false;
            bool useChains = false;

			const int x = std::get<2>(tuple).X;
			const int y = std::get<2>(tuple).Y;

            std::string tileKey(1, std::get<1>(tuple));

			if (GetChar(x, y) == '-') continue; // Skip empty tiles.
            
			if (!tileRules.contains(tileKey)) continue; // Skip if no rules exist for this tile.

			json characterRulesJson = tileRules[tileKey];

			if (!characterRulesJson.contains("Physics")) continue; // Skip if no physics rules exist for this tile.

			json characterPhysicsRulesJson = characterRulesJson["Physics"];

            if (characterPhysicsRulesJson.contains("Category"))
            {
			    const auto& categoryJson = characterPhysicsRulesJson["Category"];
                std::string categoryStr = categoryJson;
				if (categoryStr == "NONE")
				{
					category = NONE;
				}
				else if (categoryStr == "PLAYER")
				{
					category = PLAYER;
				}
				else if (categoryStr == "MAP")
				{
					category = MAP;
				}
				else if (categoryStr == "ALL")
				{
					category = ALL;
				}
            }
            if (characterPhysicsRulesJson.contains("Mask"))
            {
				const auto& maskJson = characterPhysicsRulesJson["Mask"];
                std::string maskStr = maskJson;
                if (maskStr == "NONE")
                {
                    mask = NONE;
                }
				else if (maskStr == "PLAYER")
				{
					mask = PLAYER;
                }
				else if (maskStr == "MAP")
				{
					mask = MAP;
				}
				else if (maskStr == "ALL")
				{
					mask = ALL;
				}
            }
            if (characterPhysicsRulesJson.contains("BodyType"))
            {
				const auto& bodyTypeJson = characterPhysicsRulesJson["BodyType"];
                std::string bodyTypeStr = bodyTypeJson;
                if (bodyTypeStr == "STATIC")
                {
                    bodyType = STATIC;
                }
                else if (bodyTypeStr == "DYNAMIC")
                {
                    bodyType = DYNAMIC;
                }
                else if (bodyTypeStr == "KINEMATIC")
                {
                    bodyType = KINEMATIC;
                }
                else if (bodyTypeStr == "SENSOR")
                {
                    bodyType = SENSOR;
                }
            }
		    if (characterPhysicsRulesJson.contains("UseChains"))
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
            if (characterPhysicsRulesJson.contains("Size"))
            {
				const auto& sizeJson = characterPhysicsRulesJson["Size"];
                if (sizeJson.is_array() && sizeJson.size() == 2)
                {
                    if (sizeJson[0].is_number() && sizeJson[1].is_number())
                    {
                        size.X = sizeJson[0].get<float>();
                        size.Y = sizeJson[1].get<float>();
                    }
                }
            }

            const Entity tileEntity = std::get<0>(tuple);

            if (useChains) // Chains are used on tiles, typically map tiles, to avoid ghose collisions on adjacent tiles.
            {
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

                    m_refECS.AddComponent<ChainColliderTop>(tileEntity, category, mask, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
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
					else if (hasTileBelow && hasTileDiagonalLeftBelow) { ghostX0 = (float)x - 1.0f; ghostY0 =(float) y + 1.0f; }
					else if (hasTileBelow) { ghostX0 = (float)x; ghostY0 = (float)y + 2.0f; }

					m_refECS.AddComponent<ChainColliderLeft>(tileEntity, category, mask, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
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

					m_refECS.AddComponent<ChainColliderBottom>(tileEntity, category, mask, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
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

                    m_refECS.AddComponent<ChainColliderRight>(tileEntity, category, mask, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
                        Vector2D<float>(x2, y2), Vector2D<float>(ghostX3, ghostY3));
                }
            }
            else // Create a regular physics body for the tiles that don't use chains.
            {
			    // ENGINE_INFO_D("Creating physics body for tile: " + std::to_string(tileEntity) + ", " + std::to_string(GetChar(x, y)));
                ENGINE_CRITICAL_D("Simple contacts: " + std::to_string(checkSimpleContacts));
			    std::cout << "Creating physics body for tile: " << tileEntity << ", " << GetChar(x, y) << "\n";
				m_refECS.AddComponent<PhysicsBody>(tileEntity, bodyType, category, mask,
					Vector2D<float>(size.X, size.Y),
					Vector2D<float>(static_cast<float>(x) * static_cast<float>(m_numUnitsPerTile), static_cast<float>(y) * static_cast<float>(m_numUnitsPerTile)),
					0.0f, gravityOn, checkSimpleContacts);
            }
        }
    }

    // Call this sparingly.
    void TileMap::UnloadMap()
	{
	}

    const char TileMap::GetChar(size_t x, size_t y) const
    {
        if (x < 0 || x >= m_mapDimensions.X || y < 0 || y >= m_mapDimensions.Y)
        {
            return ' ';
        }

        return m_map[y * m_mapDimensions.X + x];
    }

    const std::tuple<Entity, char, Vector2D<int>> TileMap::GetTile(size_t x, size_t y) const
    {
        // This needs to return the associated entity as well.
		if (x < 0 || x >= m_mapDimensions.X || y < 0 || y >= m_mapDimensions.Y) 
		{
			return std::make_tuple(m_refECS.INVALID_ENTITY, ' ', Vector2D<int>(x, y));
		}
		return m_tiles[y * m_mapDimensions.X + x];
	}
}
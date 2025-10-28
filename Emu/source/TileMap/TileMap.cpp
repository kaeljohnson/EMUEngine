#pragma once

#include "../../include/TileMap/TileMap.h"
#include "../../include/Logging/Logger.h"
#include "../../include/ECS/ECS.h"

// Max number of entities in the map allowed by engine.
static const int MAX_SIZE = 50000;

namespace Engine
{
	TileMap::TileMap(ECS& refECS)
		: m_refECS(refECS), m_mapDimensions(0, 0)
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

    void TileMap::CreateMap(const std::string mapFile, const std::string rulesFile)
    {
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

    const std::pair<Entity, char>* TileMap::GetTile(int x, int y) const
    {
		std::pair<int, int> key = std::make_pair(x, y);
        auto it = m_tileMap.find(key);
        if (it != m_tileMap.end())
            return &it->second; // return pointer to value

        return nullptr;
    }
}
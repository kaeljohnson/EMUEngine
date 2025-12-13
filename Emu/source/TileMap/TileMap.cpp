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

    Entity TileMap::GetEntity(size_t tileId) const
    {
		if (m_groupedEntitiesByNumMap.find(tileId) != m_groupedEntitiesByNumMap.end() && !m_groupedEntitiesByNumMap.at(tileId).empty())
		{
			return m_groupedEntitiesByNumMap.at(tileId).front(); // Return the first entity with this character.
		}

        return m_refECS.INVALID_ENTITY;
    }

    void TileMap::CreateMap(const std::string mapFile)
    {
        std::ifstream file(mapFile);
        if (!file)
        {
            ENGINE_CRITICAL("Failed to open map file: {}", mapFile);
            return;
        }

        size_t numEntities = 0;
        std::string line;
        m_mapDimensions = { 0, 0 }; // Reset before reading

        while (std::getline(file, line))
        {
            // Parse integers out of this line
            std::vector<int> tokens;
            {
                std::stringstream ss(line);
                std::string token;
                while (std::getline(ss, token, ','))
                {
                    // Trim whitespace
                    token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());

                    if (!token.empty())
                        tokens.push_back(std::stoi(token));
                }
            }

            if (tokens.empty())
                continue;

            // Set map width
            if (m_mapDimensions.Y == 0)
            {
                m_mapDimensions.X = (int)tokens.size();
            }
            else if (tokens.size() != m_mapDimensions.X)
            {
                ENGINE_CRITICAL("Map is not a rectangle on line {}", m_mapDimensions.Y + 1);
                throw std::runtime_error("Map is not a rectangle");
            }

            // Now store the integers
            for (int x = 0; x < tokens.size(); x++)
            {
                int tileValue = tokens[x];

                if (tileValue == -1)
                    continue; // don't create entity

                if (numEntities < MAX_SIZE)
                {
                    Entity tileEntity = m_refECS.CreateEntity();
                    m_tileMap[{x, m_mapDimensions.Y}] =
                        std::make_pair(tileEntity, tileValue);
                    m_groupedEntitiesByNumMap[tileValue].push_back(tileEntity);
                    numEntities++;
                }
                else
                {
                    ENGINE_CRITICAL("Map size exceeds maximum size {}", MAX_SIZE);
                }
            }

            m_mapDimensions.Y++;
        }

    }

    const std::pair<Entity, size_t>* TileMap::GetTile(int x, int y) const
    {
		std::pair<int, int> key = std::make_pair(x, y);
        auto it = m_tileMap.find(key);
        if (it != m_tileMap.end())
        {
            return &it->second; // return pointer to value
        }

        return nullptr;
    }
}
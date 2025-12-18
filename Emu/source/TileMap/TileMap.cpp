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
            // Trim whitespace from the line
            line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char c) { return !std::isspace(c); }));
            line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char c) { return !std::isspace(c); }).base(), line.end());

            if (line.empty()) // skip blank lines
                continue;

            // Remove first and last '|' if present
            if (line.front() == '|') line.erase(line.begin());
            if (!line.empty() && line.back() == '|') line.pop_back();

            if (line.empty()) // nothing left to parse
                continue;

            std::vector<int> tokens;
            std::stringstream ss(line);
            std::string token;

            while (std::getline(ss, token, '|'))
            {
                token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
                if (token.empty())
                    tokens.push_back(-1); // empty tile
                else
                    tokens.push_back(std::stoi(token));
            }

            if (tokens.empty())
                continue;

            // Set map width if first row
            if (m_mapDimensions.Y == 0)
            {
                m_mapDimensions.X = static_cast<int>(tokens.size());
            }
            else if (tokens.size() != static_cast<size_t>(m_mapDimensions.X))
            {
                ENGINE_CRITICAL("Map is not a rectangle on line {}", m_mapDimensions.Y + 1);
                throw std::runtime_error("Map is not a rectangle");
            }

            // Store tiles
            for (int x = 0; x < static_cast<int>(tokens.size()); x++)
            {
                int tileValue = tokens[x];
                if (tileValue == -1)
                    continue;

                if (numEntities >= MAX_SIZE)
                {
                    ENGINE_CRITICAL("Map size exceeds maximum size {}", MAX_SIZE);
                    break;
                }

                Entity tileEntity = m_refECS.CreateEntity();
                m_tileMap[{x, m_mapDimensions.Y}] = { tileEntity, tileValue };
                m_groupedEntitiesByNumMap[tileValue].push_back(tileEntity);
                numEntities++;
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
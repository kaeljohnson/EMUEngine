#pragma once

#include <fstream>
#include <vector>

#include "../../include/Tiles/TileMap.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	TileMap::TileMap(const std::string mapFile, const int numMetersPerTile)
        : m_mapDimensions(0, 0), m_numUnitsPerTile(numMetersPerTile)
	{
        m_map.reserve(MAX_SIZE);
        
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
	}

    void TileMap::LoadMap()
    {
        for (int y = 0; y < GetHeight(); ++y)
        {
            for (int x = 0; x < GetWidth(); ++x)
            {
                if (GetTile(x, y) != '-')
                {
                    m_tiles.emplace_back(
                        SENSOR,
						Math::Vector2D<float>(static_cast<float>(x) * static_cast<float>(m_numUnitsPerTile), static_cast<float>(y) * static_cast<float>(m_numUnitsPerTile)),
						Math::Vector2D<float>(static_cast<float>(m_numUnitsPerTile), static_cast<float>(m_numUnitsPerTile)), 0);
				}
			}
		}
	}

    void TileMap::CreateCollisionBodies()
    {
        // Creates collision bodies for the map. This creates a collision body for each block of tiles.
        // 
        // When a tile is moved or removed, the corresponding collision body needs to be updated.
        // 

        std::vector<std::vector<bool>> processed(GetHeight(), std::vector<bool>(GetWidth(), false));

        for (int y = 0; y < GetHeight(); ++y)
        {
            for (int x = 0; x < GetWidth(); ++x)
            {
                if (GetTile(x, y) != '-' && !processed[y][x])
                {
                    // Calculate the dimensions of the current block
                    int startX = x;
                    int startY = y;
                    int width = 1;
                    int height = 1;

                    // Expand horizontally to form a block
                    while (startX + width < GetWidth() && GetTile(startX + width, y) != '-' && !processed[y][startX + width])
                    {
                        ++width;
                    }

                    // Expand vertically to form a block
                    while (startY + height < GetHeight())
                    {
                        bool canMergeVertically = true;
                        for (int subX = startX; subX < startX + width; ++subX)
                        {
                            if (GetTile(subX, startY + height) == '-' || processed[startY + height][subX])
                            {
                                canMergeVertically = false;
                                break;
                            }
                        }
                        if (canMergeVertically)
                        {
                            ++height;
                        }
                        else
                        {
                            break;
                        }
                    }

                    // Mark tiles as processed
                    for (int subY = startY; subY < startY + height; ++subY)
                    {
                        for (int subX = startX; subX < startX + width; ++subX)
                        {
                            processed[subY][subX] = true;
                        }
                    }

                    // Create the tile object with calculated dimensions
                    m_collisionBodies.emplace_back(
                        STATIC,
                        Math::Vector2D<float>(static_cast<float>(startX) * static_cast<float>(m_numUnitsPerTile), static_cast<float>(startY) * static_cast<float>(m_numUnitsPerTile)),
                        Math::Vector2D<float>(static_cast<float>(width) * static_cast<float>(m_numUnitsPerTile), static_cast<float>(height) * static_cast<float>(m_numUnitsPerTile)),
                        0);
                }
            }
        }
    }

    // Call this sparingly.
    void TileMap::UnloadMap()
	{
		m_tiles.clear();
        m_tiles.shrink_to_fit();
	}

    const char TileMap::GetTile(int x, int y) const
    {
		if (x < 0 || x >= m_mapDimensions.X || y < 0 || y >= m_mapDimensions.Y) 
		{
			return ' ';
		}
		return m_map[y * m_mapDimensions.X + x];
	}
}
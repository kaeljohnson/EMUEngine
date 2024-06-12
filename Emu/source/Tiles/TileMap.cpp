#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "../../include/Tiles/TileMap.h"
#include "../../include/Logging/Logger.h"

#include "../../include/Textures/Texture.h"
#include "../../include/Textures/TextureFactory.h"

namespace Engine
{
	TileMap::TileMap(const std::string mapFile, const int numMetersPerTile)
        : m_width(0), m_height(0), m_numMetersPerTile(numMetersPerTile)
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
            if (m_height == 0) 
            {
                // Set the width to the length of the first line
                m_width = line.length();
            }
            else if (line.length() != m_width) 
            {
                // Handle error: line does not have the same length as the first line
                ENGINE_CRITICAL("Map is not a rectangle: line " + std::to_string(m_height + 1) + " has a different length");
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
            m_height++;
        }
	}

    void TileMap::LoadMap()
    {
        std::shared_ptr<ITexture> tempTextureBlue = CreateTexture(0, 0, 265);

        // Need to combine vertical tiles into one as well...
        for (size_t y = 0; y < GetHeight(); ++y)
        {
            size_t startX = 0;
            bool inBlock = false;

            for (size_t x = 0; x < GetWidth() + 1; ++x)
            {
                if (GetTile(x, y) != '-' && !inBlock)
                {
                    startX = x;
                    inBlock = true;

                    if (x == GetWidth() - 1)
                    {
                        m_tiles.emplace_back(static_cast<float>(startX) * m_numMetersPerTile, static_cast<float>(y) * m_numMetersPerTile, 1.0f * m_numMetersPerTile, 1.0f * m_numMetersPerTile, tempTextureBlue);
                    }
                }
                else if (inBlock && GetTile(x, y) != '-' && x == GetWidth() - 1)
                {
                    ENGINE_INFO_D("TileMap::LoadMap: Loading tile from x: "
                        + std::to_string(startX) + " to x: " + std::to_string(GetWidth() - 1) + " at y: " + std::to_string(y));

                    m_tiles.emplace_back(static_cast<float>(startX) * m_numMetersPerTile, static_cast<float>(y) * m_numMetersPerTile, static_cast<float>(x + 1 - startX) * m_numMetersPerTile, 1.0f * m_numMetersPerTile, tempTextureBlue);
                }
                else if ((GetTile(x, y) == '-' || x == GetWidth() - 1) && inBlock)
                {
                    ENGINE_INFO_D("TileMap::LoadMap: Loading tile from x: "
                        + std::to_string(startX) + " to x: " + std::to_string(x - 1) + " at y: " + std::to_string(y));

                    m_tiles.emplace_back(static_cast<float>(startX) * m_numMetersPerTile, static_cast<float>(y) * m_numMetersPerTile, static_cast<float>(x - startX) * m_numMetersPerTile, 1.0f * m_numMetersPerTile, tempTextureBlue);

                    inBlock = false;
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
		if (x < 0 || x >= m_width || y < 0 || y >= m_height) 
		{
			return ' ';
		}
		return m_map[y * m_width + x];
	}
}
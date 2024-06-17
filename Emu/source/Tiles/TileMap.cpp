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

    std::shared_ptr<ITexture> CreateRandomTexture()
    {
        // Generate random RGB values
        int r = rand() % 256;
        int g = rand() % 256;
        int b = rand() % 256;

        // Assuming CreateTexture takes RGB values and creates a texture
        return CreateTexture(r, g, b);
    }

    void TileMap::LoadMap()
    {
        // Need to still be able to do things like animations on a tile per tile basis.
        // 
        // Might need to move this function to somewhere like the world class right before tiles are
        // turned into physics objects. Then, there would have to be another object type other than tiles
        // that would hold all the aspects of the tile not related to collision. This would require two separate places
        // for coordinates. The underlying box2d body would be created by the code below, but the "tile" would still
        // be singular tiles so that the client can have flexibility with what happens on a per tile basis.
        // 
        // Or, have the animation system accomodate the fact that tiles might be bigger than just 1x1
        // and automatically accomodate the texture based on that. For example, 
        // if a tile is 1x3 based on a grassy tileset, the animation would automatically have the new tile 
        // consist of a left top corner grass tile, a middle grass tile, and a right grass tile.
        // 

        std::shared_ptr<ITexture> tempTextureBlue = CreateTexture(0, 0, 265);
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
                    m_tiles.emplace_back(
                        static_cast<float>(startX) * m_numMetersPerTile,
                        static_cast<float>(startY) * m_numMetersPerTile,
                        static_cast<float>(width) * m_numMetersPerTile,
                        static_cast<float>(height) * m_numMetersPerTile,
                        CreateRandomTexture());
                }
            }
        }
    }



    /*void TileMap::LoadMap()
    {
        std::shared_ptr<ITexture> tempTextureBlue = CreateTexture(0, 0, 265);

        // Need to combine vertical tiles into one as well...
        for (int y = 0; y < GetHeight(); ++y)
        {
            int startX = 0;
            bool inBlock = false;

            for (int x = 0; x < GetWidth() + 1; ++x)
            {
                if (GetTile(x, y) != '-' && !inBlock)
                {
                    startX = x;
                    inBlock = true;

                    if (x == GetWidth() - 1)
                    {
                        m_tiles.emplace_back(static_cast<float>(startX) * m_numMetersPerTile, 
                            static_cast<float>(y) * m_numMetersPerTile, 
                            1.0f * m_numMetersPerTile, 1.0f * m_numMetersPerTile, 
                            CreateRandomTexture());
                    }
                }
                else if (inBlock && GetTile(x, y) != '-' && x == GetWidth() - 1)
                {
                    ENGINE_INFO_D("TileMap::LoadMap: Loading tile from x: "
                        + std::to_string(startX) + " to x: " + std::to_string(GetWidth() - 1) + " at y: " + std::to_string(y));

                    m_tiles.emplace_back(static_cast<float>(startX) * m_numMetersPerTile, 
                        static_cast<float>(y) * m_numMetersPerTile, 
                        static_cast<float>(x + 1 - startX) * m_numMetersPerTile, 
                        1.0f * m_numMetersPerTile, 
                        CreateRandomTexture());
                }
                else if ((GetTile(x, y) == '-' || x == GetWidth() - 1) && inBlock)
                {
                    ENGINE_INFO_D("TileMap::LoadMap: Loading tile from x: "
                        + std::to_string(startX) + " to x: " + std::to_string(x - 1) + " at y: " + std::to_string(y));

                    m_tiles.emplace_back(static_cast<float>(startX) * m_numMetersPerTile, 
                        static_cast<float>(y) * m_numMetersPerTile, 
                        static_cast<float>(x - startX) * m_numMetersPerTile, 
                        1.0f * m_numMetersPerTile, 
                        CreateRandomTexture());

                    inBlock = false;
                }
            }
        }
    }*/

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
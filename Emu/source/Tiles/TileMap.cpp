#pragma once

#include "../../include/Tiles/TileMap.h"
#include "../../include/Logging/Logger.h"
#include "../../include/ECS/ECS.h"
#include "../../include/ECS/ComponentManager.h"
#include "../../include/Components.h"

namespace Engine
{
	TileMap::TileMap(ECS& refECS, const std::string mapFile, const int numMetersPerTile)
        : m_refECS(refECS), m_mapDimensions(0, 0), m_numUnitsPerTile(numMetersPerTile)
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

    std::vector<std::pair<Entity, char>>& TileMap::LoadMap()
    {
        // Created entity, character tiles.
        for (int y = 0; y < GetHeight(); ++y)
        {
            for (int x = 0; x < GetWidth(); ++x)
            {
				const char tileChar = GetChar(x, y);

                if (tileChar != '-')
                {
                    Entity tileEntity = m_refECS.CreateEntity();
                    m_refECS.AddComponent<Transform>(tileEntity,
                        Vector2D<float>(static_cast<float>(x) * static_cast<float>(m_numUnitsPerTile), static_cast<float>(y) * static_cast<float>(m_numUnitsPerTile)),
                        Vector2D<float>(static_cast<float>(m_numUnitsPerTile), static_cast<float>(m_numUnitsPerTile)),
                        1.0f, 1.0f, 1, 1);
                    m_tiles.push_back(std::make_pair(tileEntity, tileChar));
                }
                else
                {
					m_tiles.push_back(std::make_pair(999999, tileChar));

                }
            }
		}

		return m_tiles;
	}

    void TileMap::CreateCollisionBodies()
    {
        for (size_t y = 0; y < GetHeight(); ++y)
        {
            for (size_t x = 0; x < GetWidth(); ++x)
            {
				const Entity tileEntity = GetTile(x, y).first;

                if (GetTile(x, y).second != '-')
                {
                    if (GetTile(x, y - 1).second == '-' && GetTile(x, y + 1).second == '-' && GetTile(x - 1, y).second == '-' && GetTile(x + 1, y).second == '-')
                    {
                        Entity tile = m_refECS.CreateEntity();
						m_refECS.AddComponent<PhysicsBody>(tile, STATIC, MAP, PLAYER,
							Vector2D<float>(static_cast<float>(m_numUnitsPerTile), static_cast<float>(m_numUnitsPerTile)),
							Vector2D<float>(static_cast<float>(x) * static_cast<float>(m_numUnitsPerTile), static_cast<float>(y) * static_cast<float>(m_numUnitsPerTile)),
							0.0f, true, false);
                    }

					bool hasTileAbove = (y > 0 && GetTile(x, y - 1).second != '-');
					bool hasTileBelow = (y < GetHeight() - 1 && GetTile(x, y + 1).second != '-');
					bool hasTileLeft = (x > 0 && GetTile(x - 1, y).second != '-');
					bool hasTileRight = (x < GetWidth() - 1 && GetTile(x + 1, y).second != '-');

					bool hasTileDiagonalLeftAbove = (x > 0 && y > 0 && GetTile(x - 1, y - 1).second != '-');
					bool hasTileDiagonalLeftBelow = (x > 0 && y < GetHeight() - 1 && GetTile(x - 1, y + 1).second != '-');
					bool hasTileDiagonalRightAbove = (x < GetWidth() - 1 && y > 0 && GetTile(x + 1, y - 1).second != '-');
					bool hasTileDiagonalRightBelow = (x < GetWidth() - 1 && y < GetHeight() - 1 && GetTile(x + 1, y + 1).second != '-');

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

                        m_refECS.AddComponent<ChainColliderTop>(tileEntity, MAP, PLAYER, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
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

						m_refECS.AddComponent<ChainColliderLeft>(tileEntity, MAP, PLAYER, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
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

						m_refECS.AddComponent<ChainColliderBottom>(tileEntity, MAP, PLAYER, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
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

                        m_refECS.AddComponent<ChainColliderRight>(tileEntity, MAP, PLAYER, Vector2D<float>(ghostX0, ghostY0), Vector2D<float>(x1, y1),
                            Vector2D<float>(x2, y2), Vector2D<float>(ghostX3, ghostY3));
                    }
                }
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

    const std::pair<Entity, char> TileMap::GetTile(size_t x, size_t y) const
    {
        // This needs to return the associated entity as well.
		if (x < 0 || x >= m_mapDimensions.X || y < 0 || y >= m_mapDimensions.Y) 
		{
			return std::make_pair(999999, ' ');
		}
		return m_tiles[y * m_mapDimensions.X + x];
	}
}
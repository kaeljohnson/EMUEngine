#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "../../include/Tiles/TileMap.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	TileMap::TileMap(const std::string mapFile)
        : m_width(0), m_height(0)
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
}
#pragma once

#include "../Core.h"

#include <vector>
#include <string>

namespace Engine
{
	class TileMap
	{
	public:
		// Client should probably decide this.
		static const int MAX_SIZE = 1000;

		EMU_API TileMap(const std::string mapFile);

	public:
		std::vector<char> m_map;
		size_t m_width;
		size_t m_height;
	};
}
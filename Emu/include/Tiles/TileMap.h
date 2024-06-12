#pragma once

#include "../Core.h"

#include <vector>
#include <string>

#include "Tile.h"

namespace Engine
{
	class TileMap
	{
	public:
		// Client should probably decide this.
		static const int MAX_SIZE = 100000;

		EMU_API TileMap(const std::string mapFile, const int numMetersPerTile);
		EMU_API const char GetTile(int x, int y) const;
		
		EMU_API inline size_t GetWidth() const { return m_width; }
		EMU_API inline size_t GetHeight() const { return m_height; }

		void LoadMap();
		void UnloadMap();

		std::vector<Tile>::iterator begin() { return m_tiles.begin(); }
		std::vector<Tile>::iterator end() { return m_tiles.end(); }

		std::vector<Tile>::const_iterator begin() const { return m_tiles.begin(); }
		std::vector<Tile>::const_iterator end() const { return m_tiles.end(); }

	public:
		std::vector<char> m_map;
		std::vector<Tile> m_tiles;
		size_t m_width;
		size_t m_height;

		int m_numMetersPerTile;
	};
}
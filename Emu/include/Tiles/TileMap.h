#pragma once

#include <vector>

#include "../Core.h"
#include "../MathUtil.h"
#include "Tile.h"

namespace Engine
{
	class TileMap
	{
	public:
		// Client should probably decide this.
		static const int MAX_SIZE = 100000;

		EMU_API TileMap(const std::string mapFile, const int numUnitsPerTile);
		EMU_API const char GetTile(int x, int y) const;
		
		EMU_API inline int GetWidth() const { return m_mapDimensions.X; }
		EMU_API inline int GetHeight() const { return m_mapDimensions.Y; }

	public:
		inline std::vector<Tile>& GetCollisionBodies() { return m_collisionBodies; }

		void LoadMap();
		void CreateCollisionBodies();
		void UnloadMap();

		std::vector<Tile>::iterator begin() { return m_tiles.begin(); }
		std::vector<Tile>::iterator end() { return m_tiles.end(); }

		std::vector<Tile>::const_iterator begin() const { return m_tiles.begin(); }
		std::vector<Tile>::const_iterator end() const { return m_tiles.end(); }

	private:
		std::vector<char> m_map;
		std::vector<Tile> m_collisionBodies;
		std::vector<Tile> m_tiles;
		Math::Vector2D<int> m_mapDimensions;

		int m_numUnitsPerTile;
	};
}
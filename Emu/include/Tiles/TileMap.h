#pragma once

#include "../ECS/ECS.h"
#include "../Includes.h"
#include "../Core.h"
#include "../MathUtil.h"
#include "../Components.h"

namespace Engine
{
	class TileMap
	{
	public:
		// Client should probably decide this.
		static const int MAX_SIZE = 20000;

		// Tilemap should not take ECS reference. Tilemap system needs major refactoring.
		EMU_API TileMap(ECS& refECS, const std::string mapFile, const int numUnitsPerTile);
		EMU_API const char GetChar(size_t x, size_t y) const;
		EMU_API const std::pair<Entity, char> GetTile(size_t x, size_t y) const;
		
		EMU_API inline int GetWidth() const { return m_mapDimensions.X; }
		EMU_API inline int GetHeight() const { return m_mapDimensions.Y; }

	public:
		inline std::vector<PhysicsBody>& GetCollisionBodies() { return m_collisionBodies; }

		std::vector<std::pair<Entity, char>>& LoadMap();
		void CreateCollisionBodies();
		void UnloadMap();

	public:
		std::vector<char> m_map;
		std::vector<std::pair<Entity, char>> m_tiles;
		std::vector<PhysicsBody> m_collisionBodies;
		Vector2D<int> m_mapDimensions;
		Vector2D<size_t> m_sceneObjectIDs;

		int m_numUnitsPerTile;
	private:
		ECS& m_refECS;
	};
}
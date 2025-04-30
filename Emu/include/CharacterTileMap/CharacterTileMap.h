#pragma once

#include "../ECS/ECS.h"
#include "../Includes.h"
#include "../Core.h"
#include "../MathUtil.h"
#include "../Components.h"

namespace Engine
{
	class CharacterTileMap
	{
	public:
		// Client should probably decide this.
		static const int MAX_SIZE = 20000;

		// Tilemap should not take ECS reference. Tilemap system needs major refactoring.
		CharacterTileMap(ECS& refECS);
		const char GetChar(size_t x, size_t y) const;
		const std::tuple<Entity, char, Vector2D<int>> GetTile(size_t x, size_t y) const;
		
		inline int GetWidth() const { return m_mapDimensions.X; }
		inline int GetHeight() const { return m_mapDimensions.Y; }

		// Returns first tile in the map that matches the character.
		Entity GetEntity(char tileChar) const;

		// Returns all tiles in the map that match the character.
		std::vector<Entity> GetEntities(char tileChar) const;

	public:
		void CreateMap(const std::string mapFile, const std::string rulesFile);
		void LoadMap();
		void UnloadMap();

	public:
		std::vector<std::pair<Entity, char>> m_allMapEntities;
	private:
		std::vector<std::tuple<Entity, char, Vector2D<int>>> m_tiles;
		Vector2D<int> m_mapDimensions;
		int m_numUnitsPerTile;
		ECS& m_refECS;
	};
}
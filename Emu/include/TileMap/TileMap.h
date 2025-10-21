#pragma once

#include "../ECS/ECS.h"
#include "../AssetManager.h"
#include "../Includes.h"
#include "../MathUtil.h"

namespace Engine
{
	struct PairIntHash
	{
		std::size_t operator()(const std::pair<int, int>& p) const noexcept
		{
			return std::hash<long long>{}(((long long)p.first << 32) ^ (unsigned int)p.second);
		}
	};

	//using TileMap = std::unordered_map<std::pair<int, int>, std::pair<Entity, char>, PairIntHash>;

	class TileMap
	{
	public:
		TileMap(ECS& refECS, AssetManager& refAssetManager);
		~TileMap() = default;

		/*
			Gets the tile at the specified (x, y) coordinates.
			Returns a pointer to a pair containing the Entity and 
			character if found, otherwise returns nullptr.
			arg1: x - The x-coordinate of the tile.
			arg2: y - The y-coordinate of the tile.
			returns: Pointer to a pair of Entity and char, or nullptr if not found.
		*/
		const std::pair<Entity, char>* GetTile(int x, int y) const;
		
		/*
			Gets the Width of the map in tiles.
			returns: Width of the map in tiles.
		*/
		inline int GetWidth() const { return m_mapDimensions.X; }

		/*
			Gets the Height of the map in tiles.
			returns: Height of the map in tiles.
		*/
		inline int GetHeight() const { return m_mapDimensions.Y; }

		/*
			Gets the first entity that matches the character.
			Ideally used on unique tiles like player start, exit, etc.
			arg1: tileChar - The character representing the tile.
			returns: The first Entity that matches the character.
		*/
		Entity GetEntity(char tileChar) const;

		/*
			Gets all entities that match the character.
			arg1: tileChar - The character representing the tile.
			returns: A vector by reference of Entities that match the character.
		*/
		const std::vector<Entity>& GetEntities(char tileChar) const;

		/*
			Gets the entire map.
			returns: A constant reference to the map.
		*/
		const std::unordered_map<std::pair<int, int>, std::pair<Entity, char>, PairIntHash>& GetMap() const { return m_tileMap; }

		/*
			Creates the tile map from the specified map and rules files.
			arg1: mapFile - The path to the map file.
			arg2: rulesFile - The path to the rules file.
		*/
		void CreateMap(const std::string mapFile, const std::string rulesFile);

		/*
			Loads the map into the ECS by creating entities and adding components.
			This should be called in a function such as "OnScenePlay".
		*/
		void LoadMap();

		/*
			Unloads the map from the ECS by removing entities and their components.
			This should be called in a function such as "OnSceneEnd".
		*/
		void UnloadMap();

	private:
		std::unordered_map<std::pair<int, int>, std::pair<Entity, char>, PairIntHash> m_tileMap;
		std::unordered_map<char, std::vector<Entity>> m_groupedEntitiesByCharMap;

		Vector2D<int> m_mapDimensions;
		ECS& m_refECS;
		AssetManager& m_refAssetManager;
	};
}
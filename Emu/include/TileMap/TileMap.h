#pragma once

#include "../ECS/ECS.h"
#include "../../Public/Includes.h"
#include "../../Public/MathUtil.h"

namespace Engine
{
	/**
	* @struct PairIntHash
	* 
	* @brief Hash function for std::pair<int, int> to be used in unordered_map.
	*/
	struct PairIntHash
	{
		std::size_t operator()(const std::pair<int, int>& p) const noexcept
		{
			return std::hash<long long>{}(((long long)p.first << 32) ^ (unsigned int)p.second);
		}
	};

	/**
	* @class TileMap
	* 
	* @brief Manages a tile-based map within the ECS framework.
	*/
	class TileMap
	{
	public:
		/**
		* @brief Default constructor.
		*/
		TileMap();

		/**
		* @brief Constructs a TileMap with a pointer to the ECS.
		* 
		* @param ptrECS Pointer to the ECS instance.
		*/
		TileMap(ECS* ptrECS);

		/**
		* @brief Default destructor.
		*/
		~TileMap() = default;

		/**
		* @brief Creates the tile map from the specified map file.
		* 
		* @param mapFile The path to the map file.
		*/
		void CreateMap(const std::string mapFile);

		/**
		* @brief Gets the tile at the specified (x, y) coordinates.
		* Returns a pointer to a pair containing the Entity and 
		* tile ID if found, otherwise returns nullptr.
		* 
		* @param x The x-coordinate of the tile.
		* @param y The y-coordinate of the tile.
		* 
		* @return Pointer to a pair of Entity and tile ID, or nullptr if not found.
		*/
		const std::pair<Entity, size_t>* GetTile(int x, int y) const;

		/**
		* @brief Gets the width of the map in tiles.
		* 
		* @return Width of the map in tiles.
		*/
		inline int GetWidth() const { return m_mapDimensions.X; }

		/**
		* @brief Gets the height of the map in tiles.
		* 
		* @return Height of the map in tiles.
		*/
		inline int GetHeight() const { return m_mapDimensions.Y; }

		/**
		* @brief Gets the first entity that matches the tile ID.
		* Ideally used on unique tiles like player start, exit, etc.
		* 
		* @param tileId The tile ID representing the tile.
		* 
		* @return The first Entity that matches the tile ID.
		*/
		Entity GetEntity(size_t tileId) const;

		/**
		* @brief Gets all entities that match the tile ID.
		* 
		* @param tileId The tile ID representing the tile.
		* 
		* @return A const reference to a vector of Entities that match the tile ID.
		*/
		const std::vector<Entity>& GetEntities(size_t tileId) const
		{
			if (m_groupedEntitiesByNumMap.find(tileId) != m_groupedEntitiesByNumMap.end())
				return m_groupedEntitiesByNumMap.at(tileId);

			static const std::vector<Entity> emptyVector;
			return emptyVector;
		}

		/**
		* @brief Gets the entire tile map.
		* 
		* @return A constant reference to the map.
		*/
		const std::unordered_map<std::pair<int, int>, std::pair<Entity, size_t>, PairIntHash>& GetMap() const { return m_tileMap; }

	private:
		std::unordered_map<std::pair<int, int>, std::pair<Entity, size_t>, PairIntHash> m_tileMap;	/// The tile map storing tile positions and their associated entities and tile IDs.
		std::unordered_map<size_t, std::vector<Entity>> m_groupedEntitiesByNumMap;					/// Map of tile IDs to their associated entities.

		Math2D::Point2D<int> m_mapDimensions;														/// Dimensions of the map in tiles.
		ECS* m_ptrECS;																				/// Pointer to the ECS instance.
	};
}
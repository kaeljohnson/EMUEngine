#pragma once

#include "../ECS/ComponentManager.h"
#include "../ECS/ECS.h"
#include "../CharacterTileMap/CharacterTileMap.h" 
#include "../AssetManager.h"
#include "../Physics/Physics.h"
#include "../Camera/CameraSystem.h"
#include "../Includes.h"
#include "../Core.h"
#include "../MathUtil.h"

struct b2WorldId;

namespace Engine
{
	class Scene
	{
	public:
		EMU_API Scene(ECS& refECS, AssetManager& refAssetManager);
		EMU_API ~Scene();

		using ContactCallback = std::function<void(const Contact&)>;
		EMU_API void RegisterContactCallback(ContactType contactType, const char entityA, const char entityB, ContactCallback callback);

		EMU_API void RegisterContactCallback(ContactType contactType, const char entity, ContactCallback callback);

		EMU_API void SetPhysicsSimulation(const Vector2D<float> gravity);
	
		EMU_API void Add(Entity entity);

		// Removes entity from scene by removing entity
		// from entities array and deactivating entity in ECS.
		EMU_API void Activate(Entity entity);

		EMU_API void Deactivate(Entity entity);
		EMU_API void Remove(Entity entity);

		EMU_API void AddTileMap(std::string mapFileName, std::string rulesFileName);
		std::vector<std::pair<Entity, char>> GetTileMap() const { return m_tileMap.m_allMapEntities; }
		EMU_API Entity GetTileMapEntity(char tileChar) const;
		EMU_API std::vector<Entity> GetTileMapEntities(const char tileChar) const;

		// IF theres no map in the level, client will decided the dimensions manually.
		EMU_API void SetLevelDimensions(const Vector2D<int> levelWidthInUnits);

	private:
		ECS& m_refECS;

		Vector2D<int> m_levelDimensionsInUnits;
		CharacterTileMap m_tileMap;

		PhysicsSimulation m_physicsSimulation;
		CameraSystem m_cameraSystem;

		std::vector<Entity> m_entities;

	public:
		inline const int GetLevelWidth() const { return m_levelDimensionsInUnits.X; }
		inline const int GetLevelHeight() const { return m_levelDimensionsInUnits.Y; }

		void ActivatePhysics(Entity entity);
		void DeactivatePhysics(Entity entity);

		void OnScenePlay();
		void OnSceneEnd();

		void UpdateScripts();
		void UpdatePhysics();
		void UpdateCamera();
		void UpdateVisuals();

		bool HasTileMap;
	}; 
}
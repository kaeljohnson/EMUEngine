#pragma once

#include "../ECS/ComponentManager.h"
#include "../ECS/ECS.h"
#include "../TileMap/TileMap.h" 
#include "../AssetManager.h"
#include "../Physics/Physics.h"
#include "../Camera/CameraSystem.h"
#include "../Includes.h"
#include "../MathUtil.h"

struct b2WorldId;

namespace Engine
{
	class Scene
	{
	public:
		Scene(ECS& refECS, AssetManager& refAssetManager);
		~Scene();

		void RegisterOnScenePlay(std::function<void()>);

		using ContactCallback = std::function<void(const Contact&)>;
		void RegisterContactCallback(ContactType contactType, const char entityA, const char entityB, ContactCallback callback);

		void RegisterContactCallback(ContactType contactType, const char entity, ContactCallback callback);

		void SetPhysicsSimulation(const Vector2D<float> gravity);
	
		void Add(Entity entity);

		// Removes entity from scene by removing entity
		// from entities array and deactivating entity in ECS.
		void Activate(Entity entity);

		void Deactivate(Entity entity);
		void Remove(Entity entity);

		void AddTileMap(std::string mapFileName, std::string rulesFileName);
		const Entity GetTileMapEntity(char tileChar) const;
		const std::vector<Entity> GetTileMapEntities(const char tileChar) const;

		// IF theres no map in the level, client will decided the dimensions manually.
		void SetLevelDimensions(const Vector2D<int> levelWidthInUnits);

	private:
		ECS& m_refECS;
		AssetManager& m_refAssetManager;

		std::string m_mapFileName;
		std::string m_rulesFileName;

		std::function<void()> m_clientOnScenePlay;

		Vector2D<int> m_levelDimensionsInUnits;
		TileMap m_tileMap;

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

		void UpdatePhysics();
		void UpdateCamera(AssetManager& refAssetManager);

		bool HasTileMap;
	private:

		/*
			Loads all entities defined in the scene's tile map.
		*/
		void loadSceneEntitiesFromTileMap();

		/*
			Loads audio files required for the Level.
		*/
		void loadAudioFiles();
	}; 
}
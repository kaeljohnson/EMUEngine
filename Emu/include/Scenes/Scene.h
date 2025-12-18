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

		/*
			Called when the scene starts playing. loads all the map entities and 
			instantiates physics world. Also calls client defined OnScenePlay function.
		*/
		void OnScenePlay();

		/*
			Called when the scene ends. Destroys physics world and clears all entities from the scene.
		*/
		void OnSceneEnd();

		/*
			Client can register a function to be called when the scene starts playing.
		*/
		void RegisterOnScenePlay(std::function<void()>);


		using ContactCallback = std::function<void(const Contact&)>;

		/*
			Register a contact callback between two entities identified by their tileId.
			Call this function to add a behavior that should be triggered when two entities
			come into contact.
			arg1: name - The name of the scene where the contact callback is to be registered.
			arg2: contactType - The type of contact event (e.g., BEGIN_CONTACT, END_CONTACT).
			arg3: entityA - The character representing the first entity involved in the contact.
			arg4: entityB - The character representing the second entity involved in the contact.
		*/
		void RegisterContactCallback(ContactType contactType, const size_t entityA, const size_t entityB, ContactCallback callback);

		/*
			Register a contact callback for a specific entity identified by its tileId.
			Call this function to add a behavior that should be triggered when the specified
			entity comes into contact with any other entity.
			arg1: name - The name of the scene where the contact callback is to be registered.
			arg2: contactType - The type of contact event (e.g., BEGIN_CONTACT, END_CONTACT).
			arg3: entity - The character representing the entity involved in the contact.
		*/
		void RegisterContactCallback(ContactType contactType, const size_t entity, ContactCallback callback);

		/*
			Sets the physics simulation parameters for the scene.
			arg1: gravity - A Math2D::Point2D<float> representing the gravity vector for the physics simulation.
		*/
		void SetGravity(const Math2D::Point2D<float> gravity);
		// void Add(Entity entity); // No support for manually adding entities to scene for now. All entities must be added via tile map prior to runtime.
		// void Remove(Entity entity); // No support for manually removing entities from scene for now. All entities will be removed when scene ends.

		/*
			Activates an entity within the scene. There are various systems that need to be notified
			when an entity is activated so they can initialize any required data structures.
			Physics system needs to create physics bodies, ecs, etc.
			arg1: entity - The Entity to be activated.
		*/
		void Activate(Entity entity);

		/*
			Deactivates an entity within the scene. There are various systems that need to be notified
			when an entity is deactivated so they can clean up any required data structures.
			Physics system needs to destroy physics bodies, ecs, etc.
			arg1: entity - The Entity to be deactivated.
		*/
		void Deactivate(Entity entity);

		/*
			Adds a tile map to the scene. The tile map defines the layout of the level
			and the entities that should be instantiated based on the map characters.
			arg1: mapFileName - The filename of the tile map file.
			arg2: rulesFileName - The filename of the rules file that defines entity mappings.
		*/
		void AddTileMap(std::string mapFileName, std::string rulesFileName);

		/*
			Gets the entity associated with a specific character in the tile map.
			arg1: tileId - The id representing the entity in the tile map.
			returns: The Entity associated with the specified character.
		*/
		const Entity GetTileMapEntity(size_t tileId) const;

		/*
			Gets all entities associated with a specific character in the tile map.
			arg1: tileId - The character representing the entities in the tile map.
			returns: A vector of Entities associated with the specified character.
		*/
		inline const std::vector<Entity>& GetTileMapEntities(const size_t tileId) const
		{
			return m_tileMap.GetEntities(tileId);
		}

		/*
			Not really supported right now since tile map is required.
			Sets the level dimensions in units when there is no tile map.
			arg1: levelWidthInUnits - A Math::Math2D::Point2D<int> representing the width and height of the level in units.
		*/
		void SetLevelDimensions(const Math2D::Point2D<int> levelDimInUnits);

		/*
			Updates the physics simulation for the scene.
		*/
		void UpdatePhysics();

		/*
			Updates the camera system for the scene.
		*/
		void UpdateCamera(AssetManager& refAssetManager);

	private:
		ECS& m_refECS;
		AssetManager& m_refAssetManager;

		bool m_hasTileMap;
		std::string m_mapFileName;
		std::string m_rulesFileName;

		std::function<void()> m_clientOnScenePlay;

		Math2D::Point2D<int> m_levelDimensionsInUnits;
		TileMap m_tileMap;

		PhysicsSimulation m_physicsSimulation;
		CameraSystem m_cameraSystem;

		std::vector<Math2D::Chain> m_staticChains;

		std::unordered_set<Entity> m_entities;
	private:

		/*
			Activates physics for the given entity by creating its physics body/chains
			and adding it to the physics simulation.
		*/
		void activatePhysics(Entity entity);

		/*
			Deactivates physics for the given entity by destroying its physics body/chains
			and removing it from the physics simulation.
		*/
		void deactivatePhysics(Entity entity);

		/*
			Loads all entities defined in the scene's tile map.
		*/
		void loadSceneEntitiesFromTileMap();

		/*
			Loads audio files required for the Level.
		*/
		void loadAudioFiles();

		void add(Entity entity);
	}; 
}
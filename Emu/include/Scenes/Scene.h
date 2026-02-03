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
	/**
	* @class Scene
	* 
	* @brief Represents a game scene or level.
	* Holds all the management for a single runtime instance of a level. This includes the physics world, all entities in the level,
	* tile map data, camera system, etc.
	*/
	class Scene
	{
	public:
		/**
		* @brief Constructor for the Scene class.
		* 
		* @param refECS Reference to the ECS instance.
		* @param refAssetManager Reference to the AssetManager instance.
		*/
		Scene(ECS& refECS, AssetManager& refAssetManager);
		~Scene();

		/**
		* @brief Called when the scene starts playing. loads all the map entities and 
		* instantiates physics world. Also calls client defined OnScenePlay function.
		* Loads audio files and textures as well.
		*/
		void OnScenePlay();

		/**
		* @brief Called when the scene ends. Destroys physics world and clears all entities from the scene.
		* Clears all textures and audio as well.
		*/
		void OnSceneEnd();

		/**
		* @brief Client can register a function to be called when the scene starts playing.
		* 
		* @param function The function to be called when the scene starts playing.
		*/
		void RegisterOnScenePlayEvent(std::function<void()> function);

		/**
		* @brief Client can register a function to be called when the scene ends.
		* 
		* @param function The function to be called.
		*/
		void RegisterOnSceneEndEvent(std::function<void()> function);

		using ContactCallback = std::function<void(const Contact&)>; /// Type definition for contact callback functions.

		/**
		* @brief Register a contact callback between two entities identified by their tileId.
		* Call this function to add a behavior that should be triggered when two entities
		* come into contact.
		* 
		* @param contactType The type of contact event (e.g., BEGIN_CONTACT, END_CONTACT).
		* @param entityA The character representing the first entity involved in the contact.
		* @param entityB The character representing the second entity involved in the contact.
		* @param callback The callback function to be invoked when the contact event occurs.
		*/
		void RegisterContactCallback(ContactType contactType, const size_t entityA, const size_t entityB, ContactCallback callback);

		/**
		* @brief Register a contact callback for a specific entity identified by its tileId.
		* Call this function to add a behavior that should be triggered when the specified
		* entity comes into contact with any other entity.
		* 
		* @param contactType The type of contact event (e.g., BEGIN_CONTACT, END_CONTACT).
		* @param entity The character representing the entity involved in the contact.
		* @param callback The callback function to be invoked when the contact event occurs.
		*/
		void RegisterContactCallback(ContactType contactType, const size_t entity, ContactCallback callback);

		/**
		* @brief Sets the physics simulation parameters for the scene.
		*
		* @param gravity A Math2D::Point2D<float> representing the gravity vector for the physics simulation.
		*/
		void SetGravity(const Math2D::Point2D<float> gravity);

		/// void Add(Entity entity); /// @todo No support for manually adding entities to scene for now. All entities must be added via tile map prior to runtime.
		/// void Remove(Entity entity); /// @todo No support for manually removing entities from scene for now. All entities will be removed when scene ends.

		/**
		* @brief Activates an entity within the scene. There are various systems that need to be notified
		* when an entity is activated so they can initialize any required data structures.
		* Physics system needs to create physics bodies, ecs, etc.
		* 
		* @param entity The Entity to be activated.
		*/
		void Activate(Entity entity);

		/**
		* @brief Deactivates an entity within the scene. There are various systems that need to be notified
		* when an entity is deactivated so they can clean up any required data structures.
		* Physics system needs to destroy physics bodies, ecs, etc.
		* 
		* @param entity The Entity to be deactivated.
		*/
		void Deactivate(Entity entity);

		/**
		* @brief Adds a tile map to the scene. The tile map defines the layout of the level
		* and the entities that should be instantiated based on the map characters.
		*
		* @param mapFileName The filename of the tile map file.
		* @param rulesFileName The filename of the rules file that defines entity mappings.
		*/
		void AddTileMap(std::string mapFileName, std::string rulesFileName);

		/**
		* @brief Gets the entity associated with a specific character in the tile map.
		* 
		* @param tileId The id representing the entity in the tile map.
		* 
		* @return The Entity associated with the specified character.
		*/
		const Entity GetTileMapEntity(size_t tileId) const;

		/**
		* @brief Gets all entities associated with a specific character in the tile map.
		* 
		* @param tileId The character representing the entities in the tile map.
		* 
		* @return A vector of Entities associated with the specified character.
		*/
		inline const std::vector<Entity>& GetTileMapEntities(const size_t tileId) const
		{
			return m_tileMap.GetEntities(tileId);
		}

		/**
		* @brief Not really supported right now since tile map is required.
		* Sets the level dimensions in units when there is no tile map.
		* 
		* @param levelWidthInUnits A Math::Math2D::Point2D<int> representing the width and height of the level in units.
		*/
		void SetLevelDimensions(const Math2D::Point2D<int> levelDimInUnits);

		/**
		* @brief Updates the physics simulation for the scene.
		*/
		void UpdatePhysics();

		/**
		* @brief Updates the camera system for the scene.
		* 
		* @param refAssetManager Reference to the AssetManager instance.
		*/
		void UpdateCamera(AssetManager& refAssetManager);

	private:
		ECS& m_refECS;						/// Reference to the ECS instance.
		AssetManager& m_refAssetManager;	/// Reference to the AssetManager instance.

		bool m_hasTileMap;					/// Flag indicating if the scene has a tile map.
		std::string m_mapFileName;			/// The filename of the tile map file.
		std::string m_rulesFileName;		/// The filename of the rules file that defines entity mappings.

		std::vector<std::function<void()>> m_clientOnScenePlayEvents; /// Client defined functions to be called when the scene starts playing.
		std::vector<std::function<void()>> m_clientOnSceneEndEvents;  /// Client defined functions to be called when the scene ends.

		Math2D::Point2D<int> m_levelDimensionsInUnits; /// The dimensions of the level in units.
		TileMap m_tileMap;							   /// The tile map		

		PhysicsSimulation m_physicsSimulation;   	   /// The physics simulation for the scene.
		CameraSystem m_cameraSystem;				   /// The camera system for the scene.

		std::vector<Math2D::Chain> m_staticChains;	   /// Static chains created from the tile map.

		std::unordered_map<Entity, bool> m_entities;			/// Set of all entities in the scene.

		std::map<size_t, Entity> m_cameraOrder;			/// Set to render camera order correctly.
	private:

		/**
		* @brief Activates physics for the given entity by creating its physics body/chains
		* and adding it to the physics simulation.
		* 
		* @param entity The entity to activate physics for.
		*/
		void activatePhysics(Entity entity);

		/**
		* @brief Deactivates physics for the given entity by destroying its physics body/chains
		* and removing it from the physics simulation.
		* 
		* @param entity The entity to deactivate physics for.
		*/
		void deactivatePhysics(Entity entity);

		/**
		* @brief Loads all entities defined in the scene's tile map.
		*/
		void loadSceneEntitiesFromTileMap();

		/**
		* @brief Loads audio files required for the Level.
		*/
		void loadAudioFiles();

		/**
		* @brief Creates the chain collider entities that make up the map.
		* 
		* @param refChains the chains that will make up the colliders.
		*/
		void createChainColliders(std::vector<Math2D::Chain>& refChains);

		/**
		* @brief Adds an entity to the scene's entity set.
		* 
		* @param entity The entity to add.
		*/
		void add(Entity entity);
	}; 
}
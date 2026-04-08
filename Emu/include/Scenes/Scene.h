#pragma once

#include "../ECS/ComponentManager.h"
#include "../ECS/ECS.h"
#include "../TileMap/TileMap.h" 
#include "../AssetManager.h"
#include "../Physics/Physics.h"
#include "../Camera/CameraSystem.h"
#include "../../Public/Includes.h"
#include "../../Public/MathUtil.h"
#include "../Events/IOEventSystem.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct b2WorldId;

namespace Engine
{
	/**
	* @brief Represents a layer in the scene. Each layer can have its own tile map and entities.
	* The layer grouping is primarily used for rendering order and parallax effects. 
	* Examples of layers include background, physics, foreground, etc.
	*
	*/
	struct Layer
	{
		std::optional<TileMap> m_tileMap;	/// The tile map associated with the layer, if any.
		float m_parallaxFactor;				/// The parallax factor for the layer, used for parallax scrolling effects.
	};

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
		* @brief Callback function type for contact events.
		*/
		using ContactCallback = std::function<void(const Contact&)>;

		/**
		* @brief Constructs a Scene.
		* 
		* @param rulesFileName The filename of the rules JSON that defines entity mappings.
		* @param refECS Reference to the ECS instance.
		* @param refAssetManager Reference to the AssetManager instance.
		* @param refIOEventSystem Reference to the IOEventSystem instance.
		*/
		Scene(std::string rulesFileName, ECS& refECS, AssetManager& refAssetManager, IOEventSystem& refIOEventSystem);

		/**
		* @brief Destructor for the Scene class.
		*/
		~Scene();

		/**
		* @brief Called when the scene starts playing. Loads all the map entities and 
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

		/**
		* @brief Adds layers to the scene. The tile map defines the layout of the level
		* and the entities that should be instantiated based on the map characters.
		*/
		void AddLayers();

		/**
		* @brief Gets the entity associated with a specific tile ID in the tile map.
		* 
		* @param layer The layer index to query.
		* @param tileId The ID representing the entity in the tile map.
		* 
		* @return The Entity associated with the specified tile ID.
		*/
		const Entity GetTileMapEntity(int layer, size_t tileId) const;

		/**
		* @brief Gets all entities associated with a specific tile ID in the tile map.
		* 
		* @param layer The layer index to query.
		* @param tileId The ID representing the entities in the tile map.
		* 
		* @return A vector of Entities associated with the specified tile ID.
		*/
		inline const std::vector<Entity>& GetTileMapEntities(const int layer, const size_t tileId) const
		{
			return m_layers.at(layer).m_tileMap->GetEntities(tileId);
		}

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
		* @brief Sets the gravity vector for the scene's physics simulation.
		*
		* @param gravity The gravity vector for the physics simulation.
		*/
		void SetGravity(const Math2D::Point2D<float> gravity);

		/**
		* @brief Register a contact callback between two entities identified by their tile IDs.
		* Call this function to add a behavior that should be triggered when two entities
		* come into contact.
		* 
		* @param contactType The type of contact event (e.g., BEGIN_CONTACT, END_CONTACT).
		* @param tileIdA The tile ID of the first entity involved in the contact.
		* @param tileIdB The tile ID of the second entity involved in the contact.
		* @param callback The callback function to be invoked when the contact event occurs.
		*/
		void RegisterContactCallback(ContactType contactType, const Math2D::Point2D<size_t> tileIdA, const Math2D::Point2D<size_t> tileIdB, ContactCallback callback);

		/**
		* @brief Register a contact callback for a specific entity identified by its tile ID.
		* Call this function to add a behavior that should be triggered when the specified
		* entity comes into contact with any other entity.
		* 
		* @param contactType The type of contact event (e.g., BEGIN_CONTACT, END_CONTACT).
		* @param tileId The tile ID of the entity involved in the contact.
		* @param callback The callback function to be invoked when the contact event occurs.
		*/
		void RegisterContactCallback(ContactType contactType, const Math2D::Point2D<size_t> tileId, ContactCallback callback);

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

		/**
		* @brief Lets the scene know that this is an event the client cares about
		* only for this scene's runtime.
		* 
		* @param type The type of the event.
		*/
		void AddIOEvent(IOEventType type);

		/**
		* @brief Lets the scene know that this event is no longer needed in the scene's
		* runtime context.
		* 
		* @param type The type of the event.
		*/
		void RemoveIOEvent(IOEventType type);

		/// void Add(Entity entity); /// @todo No support for manually adding entities to scene for now. All entities must be added via tile map prior to runtime.
		/// void Remove(Entity entity); /// @todo No support for manually removing entities from scene for now. All entities will be removed when scene ends.

	private:
		ECS& m_refECS;						/// Reference to the ECS instance.
		AssetManager& m_refAssetManager;	/// Reference to the AssetManager instance.
		IOEventSystem& m_refIOEventSystem;	/// Reference to the io event system.

		std::string m_rulesFileName;		/// The filename of the rules file that defines entity mappings.

		std::vector<std::function<void()>> m_clientOnScenePlayEvents; /// Client defined functions to be called when the scene starts playing.
		std::vector<std::function<void()>> m_clientOnSceneEndEvents;  /// Client defined functions to be called when the scene ends.

		std::vector<Layer> m_layers;							/// Each layer. Index is the layer number.

		PhysicsSimulation m_physicsSimulation;   				/// The physics simulation for the scene.
		CameraSystem m_cameraSystem;							/// The camera system for the scene.

		std::vector<Math2D::Chain> m_staticChains;				/// Static chains created from the tile map.

		std::unordered_map<Entity, bool> m_entities;			/// Set of all entities in the scene.
		std::unordered_set<IOEventType> m_sceneRuntimeIOEvents;		/// All the IOEvents this scene has callbacks assigned to in the io system.

		std::map<size_t, Entity> m_cameraOrder;					/// Set to render camera order correctly.

		// json rules.
		json m_rulesJson; // Only one rules file per game for now so this will work.
		json m_sceneRules;
		json m_worldLayers;
		json m_sceneAssets;
		json m_componentTemplates;
		json m_characterRules;

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
		void loadSceneEntitiesFromTileMaps();

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
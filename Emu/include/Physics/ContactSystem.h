#pragma once

#include "../ECS/ECS.h"
#include "../../Public/Includes.h"
#include "../Components.h"
#include "../../Public/MathUtil.h"
#include "../TileMap/TileMap.h"
#include "../../Public/Contacts.h"
#include "../../Public/ContactKeyHashes.h"
#include "../../Public/ContactListeners.h"

namespace Engine
{
	/**
	* @class ContactSystem
	* 
	* @brief Manages contact events between entities.
	*/
	class ContactSystem
	{
	public:
		/**
		* @brief Callback function type for contact events.
		*/
		using ContactCallback = std::function<void(const Contact&)>;

		/**
		* @brief Constructs the ContactSystem with a reference to the ECS.
		*
		* @param refECS Reference to the ECS.
		*/
		ContactSystem(ECS& refECS);

		/**
		* @brief Default destructor.
		*/
		~ContactSystem() = default;

		/**
		* @brief Sets the number of layers for the contact system. This is needed by the contact system to know how many tile maps to expect and manage.
		* 
		* @param numLayers The number of layers in the tile map.
		*/
		void SetNumLayers(size_t numLayers);

		/**
		* @brief Adds a TileMap pointer for a specific layer number. This allows the ContactSystem 
		* to manage multiple layers of tile maps, which is essential for handling contacts in a multi-layered environment.
		* 
		* @param layerNum The layer number for which the TileMap is being added.
		* @param ptrTileMap Pointer to the TileMap to be added for the specified layer.
		*/
		void AddTileMap(size_t layerNum, TileMap* ptrTileMap) { m_tileMaps[layerNum] = ptrTileMap; }

		/**
		* @brief Registers a contact callback for a specific contact type and tile ID.
		* 
		* @param contactType The type of contact event.
		* @param tileIdA The tile ID of the first entity.
		* @param callback The callback function to register.
		*/
		void RegisterContactCallback(ContactType contactType, const Math2D::Point2D<size_t> tileIdA, ContactCallback callback);

		/**
		* @brief Registers a contact callback for a specific contact type and tile IDs.
		* 
		* @param contactType The type of contact event.
		* @param tileIdA The tile ID of the first entity.
		* @param tileIdB The tile ID of the second entity.
		* @param callback The callback function to register.
		*/
		void RegisterContactCallback(ContactType contactType, const Math2D::Point2D<size_t> tileIdA, const Math2D::Point2D<size_t> tileIdB, ContactCallback callback);

		/**
		* @brief Activates a contact callback for a specific contact type and entity.
		* 
		* @param contactType The type of contact event.
		* @param entityA The entity involved in the contact.
		* @param callback The callback function to activate.
		*/
		void ActivateContactCallback(ContactType contactType, Entity entityA, ContactCallback callback);

		/**
		* @brief Activates a contact callback for a specific contact type and entities.
		* 
		* @param contactType The type of contact event.
		* @param entityA The first entity involved in the contact.
		* @param entityB The second entity involved in the contact.
		* @param callback The callback function to activate.
		*/
		void ActivateContactCallback(ContactType contactType, Entity entityA, Entity entityB, ContactCallback callback);

		/**
		* @brief Activates all registered contact callbacks.
		*/
		void ActivateContactCallbacks();

		/**
		* @brief Processes all contact events.
		* 
		* @param ptrWorldId Pointer to the world ID.
		*/
		void ProcessContacts(void* ptrWorldId);

		/**
		* @brief Cleans up the contact system.
		*/
		void Cleanup();

	private:
		ECS& m_refECS;				/// Reference to the ECS

		std::vector<TileMap*> m_tileMaps;	/// Vector of pointers to TileMaps for each layer

		std::vector<std::tuple<ContactType, const Math2D::Point2D<size_t>, ContactCallback>> m_singleEntityContactCallbacks;			/// Single entity contact callbacks
		std::vector<std::tuple<ContactType, const Math2D::Point2D<size_t>, const Math2D::Point2D<size_t>, ContactCallback>> m_multiContactCallbacks;	/// Multi-entity contact callbacks

		std::unordered_map<SingleEntityBeginContactKey, ContactCallback> m_beginSingleEntityContactCallbacks;		/// Single entity begin contact callbacks
		std::unordered_map<SingleEntityBeginContactKey, ContactCallback> m_beginSingleEntitySensingCallbacks;		/// Single entity begin sensing callbacks
		std::unordered_map<SingleEntityEndContactKey, ContactCallback> m_endSingleEntityContactCallbacks;			/// Single entity end contact callbacks
		std::unordered_map<SingleEntityEndContactKey, ContactCallback> m_endSingleEntitySensingCallbacks;			/// Single entity end sensing callbacks
		std::unordered_map<MultiEntityBeginContactKey, ContactCallback> m_beginMultiEntityContactCallbacks;			/// Multi-entity begin contact callbacks
		std::unordered_map<MultiEntityBeginContactKey, ContactCallback> m_beginMultiEntitySensingCallbacks;			/// Multi-entity begin sensing callbacks
		std::unordered_map<MultiEntityEndContactKey, ContactCallback> m_endMultiEntityContactCallbacks;				/// Multi-entity end contact callbacks
		std::unordered_map<MultiEntityEndContactKey, ContactCallback> m_endMultiEntitySensingCallbacks;				/// Multi-entity end sensing callbacks
	};
}
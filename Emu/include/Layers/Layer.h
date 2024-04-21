#pragma once

#include <string>

#include "../Core.h"
#include "../Events/Event.h"
#include "../GameObjects/GameObject.h"
#include "../CallbackSystem/CallbackSystem.h"

namespace Engine
{

	// Layer class. Layers are used to group scene objects. 
	// Layers can help with oranizing objects 
	// in the game for rendering and event processing.

	class Layer
	{
	public:
		bool IsAttachedToScene;

	protected:
		std::string m_name;
		bool m_enabled;

		// Note: common box2d bug where collisions won't work if dynamic objects are
		// created and simulated before static objects.
		std::vector<GameObject*> m_gameObjects;

		ICallbackSystem* ptrICallbackSystem;
	public:
		EMU_API Layer(std::string name);
		EMU_API virtual	~Layer();

		const std::vector<GameObject*>::iterator begin() { return m_gameObjects.begin(); }
		const std::vector<GameObject*>::iterator end() { return m_gameObjects.end(); }


		// Adds the layer to the world. This probably should 
		// not be a layer member function. Once scenes exist, 
		// it can be something like "scene->AddLayer(layer)".
		EMU_API void AddToWorld();

		// Removes the layer from the world.
		// Should be a scene member function.
		EMU_API void RemoveFromWorld();

		// Does anything the client might need to do when the layer is attached.
		EMU_API virtual void OnAttach();

		// Does anything the client might need to do when the layer is detached.
		EMU_API virtual void OnDetach();

		// Does anything the client might need to do when the layer is freed.
		EMU_API virtual void Free();

		// Does anything the client might need to do when the layer is updated.
		EMU_API virtual void OnUpdate();

		// Does anything the client might need to do when an event is processed.
		EMU_API virtual void ProcessEvent(Event& e);

		// Returns the name of the layer.
		const std::string& GetName() const;

		// Add a game object to the layer.
		EMU_API void AddGameObject(GameObject* gameObject);

		// Remove a game object from the layer.
		EMU_API void RemoveGameObject(GameObject* gameObject);
	};
}
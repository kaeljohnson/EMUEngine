#pragma once

#include <string>
#include "Event.h"

#include "../Core.h"
#include "../Scenes/SceneObject.h"
#include "../CallbackSystem/CallbackSystem.h"

namespace Engine
{
	// Event listener class. Listeners are used to listen for events and process them.
	// Listeners can subscribe to scenes.

	class EventListener
	{
	public:
		std::string m_name;
		bool IsAttachedToScene;

		bool m_enabled;
	protected:

		// Note: common box2d bug where collisions won't work if dynamic objects are
		// created and simulated before static objects.
		std::vector<SceneObject*> m_sceneObjects;

		ICallbackSystem* ptrICallbackSystem;
	public:
		EMU_API EventListener(std::string name);
		EMU_API virtual	~EventListener();

		const std::vector<SceneObject*>::iterator begin() { return m_sceneObjects.begin(); }
		const std::vector<SceneObject*>::iterator end() { return m_sceneObjects.end(); }

		// Does anything the client might need to do when an event is processed.
		EMU_API virtual void ProcessEvent(Event& e);

		// Add a scene object to the listener.
		EMU_API void Subscribe(SceneObject* sceneObject);

		// Remove a scene object from the listener.
		EMU_API void Unsubscribe(SceneObject* sceneObject);
	};
}
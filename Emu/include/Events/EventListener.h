#pragma once

#include <string>
#include "Event.h"

#include "../Core.h"
#include "../CallbackSystem/CallbackSystem.h"

namespace Engine
{
	// Event listener class. Listeners are used to listen for events and process them.
	// Listeners can subscribe to scenes.

	class EventListener
	{
	public:
		std::string m_name;
		bool m_enabled;

		bool IsAttachedToApp;

	protected:
		ICallbackSystem* ptrICallbackSystem;

	public:
		EMU_API EventListener(std::string name);
		EMU_API virtual	~EventListener() = default;

		// Does anything the client might need to do when an event is processed.
		EMU_API virtual void ProcessEvent(Event& e);
	};
}
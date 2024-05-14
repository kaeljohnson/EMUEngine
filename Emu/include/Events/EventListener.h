#pragma once

#include <string>
#include "Event.h"

#include "../Core.h"
#include "../CallbackSystem/CallbackSystem.h"

namespace Engine
{
	// Event listener class. Listeners are used to listen for events and process them.
	// Listeners use the apps callback system to trigger events.

	class EventListener
	{
	private:
		bool m_enabled;

	protected:
		ICallbackSystem* ptrICallbackSystem;

	public:
		EMU_API EventListener();
		EMU_API virtual	~EventListener() = default;

		// Must design function carefully when manipulating objects tied to a scene as the 
		// event listeners are not tied to a scene directly. 
		// Maybe change this in the future where an event listener can be tied to a scene 
		// or directly to the app.
		EMU_API virtual void ProcessEvent(Event& e);

		EMU_API const bool IsEnabled() const;
	};
}
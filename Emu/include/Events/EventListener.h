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
		EMU_API virtual void ProcessEvent(Event& e);

		EMU_API const bool IsEnabled() const;
	};
}
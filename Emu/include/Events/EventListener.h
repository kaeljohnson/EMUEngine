#pragma once

#include <string>

#include "../Core.h"

#include "Event.h"

namespace Engine
{
	// Event listener class. Listeners are used to listen for events and process them.

	class EventListener
	{
	public:
		bool Enabled;
	public:
		EMU_API EventListener();
		EMU_API virtual	~EventListener() = default;
		EMU_API virtual void ProcessEvent(Event& e);
	};
}
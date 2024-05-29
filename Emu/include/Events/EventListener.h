#pragma once

#include <string>
#include "Event.h"

#include "../Core.h"

namespace Engine
{
	// Event listener class. Listeners are used to listen for events and process them.

	class EventListener
	{
	private:
		bool m_enabled;

	public:
		EMU_API EventListener();
		EMU_API virtual	~EventListener() = default;

		EMU_API virtual void ProcessEvent(Event& e);

		EMU_API const bool IsEnabled() const;
	};
}
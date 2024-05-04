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
		std::string m_name;
		bool m_enabled;

		bool m_attachedToApp;

	protected:
		ICallbackSystem* ptrICallbackSystem;

	public:
		EMU_API EventListener(std::string name);
		EMU_API virtual	~EventListener() = default;
		EMU_API virtual void ProcessEvent(Event& e);

		EMU_API const std::string GetName() const;
		EMU_API const bool IsEnabled() const;
		EMU_API const bool IsAttached() const;
		EMU_API void SetAttached(bool attached);
	};
}
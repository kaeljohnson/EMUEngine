#pragma once

#include "../Layers/Layer.h"
#include "../Events/Event.h"
#include "../Events/IEventSystem.h"

namespace Engine
{
	class WindowManagerLayer : public Layer
	{
	private:
		IEventSystem* ptrEventSystem;
	public:
		WindowManagerLayer(IEventSystem* eventSystem);
		~WindowManagerLayer();

		void onAttach() override;
		void onDetach() override;
		void onUpdate() override;
		void processEvent(Event& e) override;
	};
}
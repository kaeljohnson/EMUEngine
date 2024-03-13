#pragma once

#include "../Layers/Layer.h"
#include "../Events/Event.h"
#include "../Events/IEventAction.h"

namespace Engine
{
	class WindowManagerLayer : public Layer
	{
	private:
		IEventAction* ptrEventSystem;
	public:
		WindowManagerLayer(IEventAction* eventSystem);
		~WindowManagerLayer();

		void onAttach() override;
		void onDetach() override;
		void onUpdate() override;
		void processEvent(Event& e) override;
	};
}
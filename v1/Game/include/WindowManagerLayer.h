#pragma once

#include "EngineIncludes.h"

namespace Engine
{
	class WindowManagerLayer : public Layer
	{
	private:
		IEventSystem& refEventSystem;
	public:
		WindowManagerLayer(IEventSystem& eventSystem);
		~WindowManagerLayer();

		void onAttach() override;
		void onDetach() override;
		void onUpdate() override;
		void processEvent(Event& e) override;
	};
}
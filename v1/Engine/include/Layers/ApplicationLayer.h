#pragma once

#include "../Layers/Layer.h"
#include "../Events/IEventSystem.h"

namespace Engine
{ 
	class ApplicationLayer : public Layer
	{
	private:
		IEventSystem* ptrEventSystem;
	public:
		ApplicationLayer(IEventSystem* eventSystem);
		~ApplicationLayer();

		void onAttach() override;
		void onDetach() override;
		void onUpdate() override;
		void processEvent(Engine::Event& e) override;
};
} 
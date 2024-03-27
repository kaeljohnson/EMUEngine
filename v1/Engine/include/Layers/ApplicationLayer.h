#pragma once

#include "../Layers/Layer.h"
#include "../Events/Event.h"
#include "../Events/IEventAction.h"
#include "ILayerEvent.h"
#include "LayerEvents.h"

namespace Engine
{ 
	class ApplicationLayer : public Layer
	{
	private:
		IEventAction* ptrEventSystem;
	public:
		ApplicationLayer(IEventAction* eventSystem, ILayerEvent* ptrLayerEventSystem);
		~ApplicationLayer();

		void onAttach() override;
		void onDetach() override;
		void onUpdate() override;
		void processEvent(Engine::Event& e) override;
};
} 
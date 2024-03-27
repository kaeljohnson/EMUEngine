#pragma once

#include "../Layers/Layer.h"
#include "../Events/Event.h"
#include "../CallbackSystem/CallbackSystem.h"


namespace Engine
{ 
	class ApplicationLayer : public Layer
	{
	private:
		ICallbackSystem* ptrEventSystem;
	public:
		ApplicationLayer(ICallbackSystem* ptrICallbackSystem);
		~ApplicationLayer();

		void onAttach() override;
		void onDetach() override;
		void onUpdate() override;
		void processEvent(Engine::Event& e) override;
};
} 
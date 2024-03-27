#pragma once

#include "../Layers/Layer.h"
#include "../Events/Event.h"
#include "../CallbackSystem/CallbackSystem.h"
namespace Engine
{
	class WindowManagerLayer : public Layer
	{
	private:
		ICallbackSystem* ptrICallbackSystem;
	public:
		WindowManagerLayer(ICallbackSystem* ptrICallbackSystem);
		~WindowManagerLayer();

		void onAttach() override;
		void onDetach() override;
		void onUpdate() override;
		void processEvent(Event& e) override;
	};
}
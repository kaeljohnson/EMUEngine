#pragma once 

#include <queue>

#include "Layer.h"
#include "../Events/Event.h"

namespace Engine
{
	class DebugLayer : public Layer
	{
	private:
		// Should this be a vector?
		// std::vector<DebugObjects> debugObjects;
	public:
		DebugLayer(std::queue<Event>& eventQ);
		~DebugLayer() = default;

		void processEvents();
		void update();
		void render();
		void display();

	};
}
#pragma once 

#include <queue>

#include "Layer.h"
#include "../Events/Event.h"

namespace Engine
{
	class ForegroundLayer : public Layer
	{
	private:
		// Should this be a vector?
		// std::vector<foregroundObject> foregroundObjects;
	public:
		ForegroundLayer(std::queue<Event>& eventQ);
		~ForegroundLayer() = default;

		void processEvents();
		void update();
		void render();
		void display();

	};
}
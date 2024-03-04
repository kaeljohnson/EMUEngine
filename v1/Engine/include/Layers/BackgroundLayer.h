#pragma once 

#include <queue>

#include "Layer.h"
#include "../Events/Event.h"

namespace Engine
{
	class BackgroundLayer : public Layer
	{
	private:
		// Should this be a vector?
		// std::vector<BackgroundObject> backgroundObjects;
	public:
		BackgroundLayer(std::queue<Event>& eventQ);
		~BackgroundLayer() = default;

		void processEvents();
		void update();
		void render();
		void display();
	};
}
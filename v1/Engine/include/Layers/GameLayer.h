#pragma once 

#include <queue>

#include "Layer.h"
#include "../Events/Event.h"

namespace Engine
{
	class GameLayer : public Layer
	{
	private:
		// Should this be a vector?
		// std::vector<GameObject> gameObjects;
	public:
		GameLayer(std::queue<Event>& eventQ);
		~GameLayer() = default;

		void processEvents();
		void update();
		void render();
		void display();

	};
}
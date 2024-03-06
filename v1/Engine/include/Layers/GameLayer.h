#pragma once 

#include <queue>

#include "Layer.h"
#include "../Events/Event.h"
#include "../GameObjects/GameObject.h"

namespace Engine
{
	class GameLayer : public Layer
	{
	private:
		// Instead of a vector, the client could specify how many game
		// objects they want on the screen at once.This would be better than a vector.
		std::vector<GameObject> m_gameObjects;
	public:
		GameLayer(std::queue<Event>& eventQ);
		~GameLayer() = default;

		void push(GameObject& gameObject);

		void processEvents();
		void update();
		void render();
		void display();
	};
}
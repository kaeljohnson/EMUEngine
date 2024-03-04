#pragma once

#include <queue>

#include "../Events/Event.h"
#include "../Events/Event.h"

namespace Engine
{
	class Layer
	{
	public:
		std::queue<Event>& refEventQ;

		Layer(std::queue<Event>& eventQ);
		~Layer() = default;

		virtual void processEvents() = 0;
	};
}


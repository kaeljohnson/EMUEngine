#pragma once

#include <queue>

#include "../../include/Layers/Layer.h"

namespace Engine
{
	Layer::Layer(std::queue<Event>& eventQ)
		: refEventQ(eventQ)
	{

	}
}
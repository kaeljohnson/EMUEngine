#pragma once

#include "../Includes.h"
#include "Scene.h"

namespace Engine
{
	inline std::shared_ptr<Scene> CreateScene()
	{
		return std::make_shared<Scene>();
	}
}
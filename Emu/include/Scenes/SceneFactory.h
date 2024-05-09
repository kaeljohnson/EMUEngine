#pragma once

#include <memory>

#include <string>

namespace Engine
{
	inline std::shared_ptr<Scene> CreateScene(const std::string& name, float timeStep, int pixelsPerMeter)
	{
		return std::make_shared<Scene>(name, timeStep, pixelsPerMeter);
	}
}
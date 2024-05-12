#pragma once

#include <memory>

#include <string>

namespace Engine
{
	inline std::shared_ptr<Scene> CreateScene()
	{
		return std::make_shared<Scene>();
	}
}
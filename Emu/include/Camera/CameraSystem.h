#pragma once

#include "../ECS/ECS.h"
#include "../AssetManager.h"
#include "../MathUtil.h"

namespace Engine
{
	class CameraSystem
	{
	public:
		CameraSystem(ECS& refECS);
		~CameraSystem() = default;

		void Frame(const Vector2D<int> mapBounds);
		void Update(AssetManager& refAssetManager);

	private:
		ECS& m_refECS;
	};
}
#pragma once

#include <iostream>

#include "../include/Logging/Logger.h"

#include "../Include/Transform.h"
#include "../include/ComponentManager/ComponentManager.h"


namespace Engine
{
	Transform::Transform(const size_t id)
		: PrevPosition(0.0f, 0.0f), Position(0.0f, 0.0f),
		Dimensions(0.0f, 0.0f), Rotation(0.0f), Scale(1.0f), DirectionFacing(1), Component(id)
	{
		ENGINE_INFO_D("Transform component created with ID: " + std::to_string(id));
		// ComponentManagerRegistry::GetManager<Transform>().AddComponent(id, this);

	}

	Transform::Transform(const size_t id, Vector2D<float> position, Vector2D<float> dimensions, float rotation, float scale, int direction)
		: PrevPosition(position), Position(position), 
		Dimensions(dimensions), Rotation(rotation), Scale(scale), DirectionFacing(direction), Component(id)
	{}

	Transform::~Transform()
	{
		ENGINE_INFO_D("Transform component destroyed with ID: " + std::to_string(m_id));
	}
}
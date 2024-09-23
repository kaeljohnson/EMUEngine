#pragma once

#include <iostream>

#include "../include/Logging/Logger.h"

#include "../Include/Transform.h"

// Circular dependency
#include "../include/ComponentManager/ComponentManager.h"


namespace Engine
{
	Transform::Transform() : m_id(0), PrevPosition(0.0f, 0.0f), Position(0.0f, 0.0f), 
		Dimensions(0.0f, 0.0f), Rotation(0.0f), Scale(1.0f), DirectionFacing(1)
	{
		// std::cout << "FROM TRANSFORM APP: ADDRESS: " << this << std::endl;
		ENGINE_CRITICAL_D("Transform component created without an ID");
	}

	Transform::Transform(const size_t id)
		: m_id(id), PrevPosition(0.0f, 0.0f), Position(0.0f, 0.0f),
		Dimensions(0.0f, 0.0f), Rotation(0.0f), Scale(1.0f), DirectionFacing(1)
	{
		ENGINE_INFO_D("Transform component created with ID: " + std::to_string(id));
		// std::cout << "FROM TRANSFORM APP: ADDRESS: " << this << std::endl;
	}

	Transform::Transform(const size_t id, Vector2D<float> position, Vector2D<float> dimensions, float rotation, float scale, int direction)
		: m_id(id), PrevPosition(position), Position(position), Dimensions(dimensions), Rotation(rotation), Scale(scale), DirectionFacing(direction)
	{
	}

	void Transform::AttachToSceneObject(const size_t id)
	{
		m_id = id;
		ComponentManagerRegistry::GetManager<Transform>().AddComponent(id, this);
	}
}
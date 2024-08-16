#pragma once

#include <memory>

#include "../../include/CommonFunctions.h"
#include "../../include/Scenes/SceneObject.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Textures/ITexture.h"
#include "../../include/Events/EventManager.h"

namespace Engine
{
	SceneObject::SceneObject(const BodyType bodyType, const bool fixed, const Vector2D<float> position,
		const Vector2D<float> size, int directionFacing)
		: m_physicsBody(std::make_shared<PhysicsBody>(bodyType, fixed, position, size)),
		Enabled(true), Visible(true), LayerIdx(-1), uuid(CreateUUID()), DirectionFacing(directionFacing)
	{
		ENGINE_INFO_D("SceneObject created");
	}

	void SceneObject::UpdatePrevPosition()
	{ 
		// If we can be sure that the object has not moved, 
		// we don't have to update the previous position.
		m_physicsBody->UpdatePrevPosition();
	}

	void SceneObject::EngineSideUpdate()
	{
		// World updates physics bodies.
		switch (m_physicsBody->GetBodyType())
		{
			case DYNAMIC:
				m_physicsBody->UpdatePrevPosition();
				m_physicsBody->SetContactFlags();
				break;
			case STATIC:
				m_physicsBody->SetContactFlagsToFalse();
				break;
			case SENSOR:
				m_physicsBody->SetContactFlagsToFalse();
				break;
			default:
				ENGINE_WARN_D("Body type has no engine side updating.");
				break;
		}
	}

	void SceneObject::Update()
	{
		// Purely virtual function meant to be implemented by client of engine.
	}
}
#pragma once

#include <memory>

#include "../../include/CommonFunctions.h"
#include "../../include/Scenes/SceneObject.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Textures/ITexture.h"
#include "../../include/Physics/PhysicsFactory.h"
#include "../../include/Events/EventManager.h"

namespace Engine
{
	SceneObject::SceneObject(const BodyType bodyType, const bool fixed, const float startingXInMeters, const float startingYInMeters,
		const float widthInMeters, const float heightInMeters, std::shared_ptr<ITexture> ptrTexture)
		: m_physicsBody(CreatePhysicsBody(bodyType, fixed, startingXInMeters, startingYInMeters, widthInMeters, heightInMeters)),
		m_texture(ptrTexture), Enabled(true), Visible(true), uuid(CreateUUID()),
		refKeyStates(EventManager::GetInstance()->GetKeyStates())
	{
		ENGINE_INFO_D("SceneObject created");
	}

	void SceneObject::SetXVelocity(const float xVel) { m_physicsBody->SetXVelocity(xVel); }
	void SceneObject::SetYVelocity(const float yVel) { m_physicsBody->SetYVelocity(yVel); }
	void SceneObject::SetGravity(bool enabled) { m_physicsBody->SetGravity(enabled); }

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
				m_physicsBody->SetCollisionFlags();
				break;
			case STATIC:
				m_physicsBody->SetCollisionFlagsToFalse();
				break;
			case SENSOR:
				m_physicsBody->SetCollisionFlagsToFalse();
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
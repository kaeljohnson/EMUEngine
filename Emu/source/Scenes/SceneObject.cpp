#pragma once

#include <memory>

#include "../../include/CommonFunctions.h"
#include "../../include/Scenes/SceneObject.h"
#include "../../include/Logging/ILogger.h"
#include "../../include/Textures/ITexture.h"
#include "../../include/Physics/PhysicsFactory.h"

namespace Engine
{
	SceneObject::SceneObject(const BodyType bodyType, const bool fixed, const float startingXInMeters, const float startingYInMeters,
		const float widthInMeters, const float heightInMeters, std::shared_ptr<ITexture> ptrTexture)
		: m_physicsBody(CreatePhysicsBody(bodyType, fixed, startingXInMeters, startingYInMeters, widthInMeters, heightInMeters)),
		m_texture(ptrTexture), Enabled(true), Visible(true), uuid(CreateUUID())
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

	void SceneObject::Update()
	{
		// World updates physics bodies.
	}
}
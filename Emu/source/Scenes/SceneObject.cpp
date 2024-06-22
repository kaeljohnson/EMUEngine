#pragma once

#include <memory>

#include "../../include/CommonFunctions.h"
#include "../../include/Scenes/SceneObject.h"
#include "../../include/Logging/Logger.h"
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

	void SceneObject::CheckAllCollisions()
	{
		// Engine uses this for checking if 
		// Entities are on the ground, ceiling, left wall, or right wall
		// each frame.

		// m_physicsBody->SetIsBeingCollidedWith(false);
		// m_physicsBody->SetIsCollidingWith(false);
		m_physicsBody->SetBottomCollision(false);
		m_physicsBody->SetTopCollision(false);
		m_physicsBody->SetLeftCollision(false);
		m_physicsBody->SetRightCollision(false);

		m_physicsBody->SetBottomSensor(false);
		m_physicsBody->SetTopSensor(false);
		m_physicsBody->SetLeftSensor(false);
		m_physicsBody->SetRightSensor(false);
	}
}
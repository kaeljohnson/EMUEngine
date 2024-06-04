#pragma once

#include <memory>

#include "../../include/CommonFunctions.h"
#include "../../include/Scenes/SceneObject.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Textures/Texture.h"
#include "../../include/Physics/IPhysicsBody.h"
#include "../../include/Physics/PhysicsFactory.h"

namespace Engine
{
	SceneObject::SceneObject(const BodyType bodyType, const bool fixed, const float startingXInMeters, const float startingYInMeters,
		const float widthInMeters, const float heightInMeters, Texture& refTexture)
		: m_physicsBody(CreatePhysicsBody(bodyType, fixed, startingXInMeters, startingYInMeters, widthInMeters, heightInMeters)), 
		m_texture(&refTexture), m_enabled(true), m_visible(true), uuid(CreateUUID())
	{
		ENGINE_INFO_D("SceneObject created");
	}

	const bool SceneObject::IsEnabled() const { return m_enabled; }

	std::string SceneObject::GetUUID() { return uuid; }

	void SceneObject::SetXVelocity(const float xVel) { m_physicsBody->SetXVelocity(xVel); }
	void SceneObject::SetYVelocity(const float yVel) { m_physicsBody->SetYVelocity(yVel); }
	void SceneObject::SetGravity(bool enabled) { m_physicsBody->SetGravity(enabled); }
	void SceneObject::SetVisibility(const bool visible) { m_visible = visible; }
	std::shared_ptr<IPhysicsBody> SceneObject::GetPhysicsBody() { return m_physicsBody; }
	const float SceneObject::GetXVelocity() const { return m_physicsBody->GetXVelocity(); }
	const float SceneObject::GetYVelocity() const { return m_physicsBody->GetYVelocity(); }

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
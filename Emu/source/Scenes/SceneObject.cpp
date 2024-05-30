#pragma once

#include <memory>

#include "../../include/CommonFunctions.h"
#include "../../include/Scenes/SceneObject.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Textures/Texture.h"

namespace Engine
{
	SceneObject::SceneObject(std::shared_ptr<IPhysicsBody> ptrPhysicsBody, Texture& ptrTexture)
		: uuid(CreateUUID()), m_physicsBody(ptrPhysicsBody), m_texture(&ptrTexture), m_enabled(true), m_visible(true)
	{
		ENGINE_INFO_D("SceneObject created");
	}

	const bool SceneObject::IsEnabled() const { return m_enabled; }

	std::string SceneObject::GetUUID() { return uuid; }

	void SceneObject::SetXVelocity(const float xVel) { m_physicsBody->SetXVelocity(xVel); }
	void SceneObject::SetYVelocity(const float yVel) { m_physicsBody->SetYVelocity(yVel); }
	void SceneObject::SetVisibility(const bool visible) { m_visible = visible; }
	std::shared_ptr<IPhysicsBody> SceneObject::GetPhysicsBody() { return m_physicsBody; }
	Texture* SceneObject::GetTexture() { return m_texture; }

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
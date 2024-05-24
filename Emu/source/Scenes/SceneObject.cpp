#pragma once

#include <memory>

#include "../../include/CommonFunctions.h"
#include "../../include/Scenes/SceneObject.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Textures/Texture.h"

namespace Engine
{
	SceneObject::SceneObject(std::shared_ptr<IPhysicsBody> ptrPhysicsBody, Texture* ptrTexture)
		: uuid(CreateUUID()), m_physicsBody(ptrPhysicsBody), m_texture(ptrTexture), m_enabled(true)
	{
		ENGINE_INFO_D("SceneObject created");
	}

	Texture* SceneObject::GetTexture()
	{
		return m_texture;
	}

	std::shared_ptr<IPhysicsBody> SceneObject::GetPhysicsBody()
	{
		return m_physicsBody;
	}

	std::string SceneObject::GetUUID()
	{
		return uuid;
	}

	const bool SceneObject::IsEnabled() const { return m_enabled;  }

	void SceneObject::SetXVelocity(const float xVel)
	{
		m_physicsBody->SetXVelocity(xVel);
	}

	void SceneObject::SetYVelocity(const float yVel)
	{
		m_physicsBody->SetYVelocity(yVel);
	}

	void SceneObject::update()
	{
		// World updates physics bodies.
	}
}
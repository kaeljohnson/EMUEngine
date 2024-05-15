#pragma once

#include <memory>

#include "../../include/Scenes/SceneObject.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Textures/Texture.h"

namespace Engine
{
	SceneObject::SceneObject(std::shared_ptr<IPhysicsBody> ptrPhysicsBody, Texture* ptrTexture) 
		: m_physicsBody(ptrPhysicsBody), m_texture(ptrTexture), m_enabled(true)
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

	const bool SceneObject::IsEnabled() const { return m_enabled;  }
}
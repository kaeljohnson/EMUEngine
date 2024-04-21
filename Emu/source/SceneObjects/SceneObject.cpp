#pragma once

#include "../../include/SceneObjects/SceneObject.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/BodyTypes.h"
#include "../../include/Textures/Texture.h"

namespace Engine
{
	SceneObject::SceneObject(IPhysicsBody* ptrPhysicsBody, Texture* ptrTexture) : m_physicsBody(ptrPhysicsBody), m_texture(ptrTexture)
	{
		ENGINE_INFO_D("SceneObject created");
	}

	SceneObject::~SceneObject()
	{
		ENGINE_INFO_D("Freeing SceneObject!");
		if (m_physicsBody != nullptr)
			delete m_physicsBody;

		//delete m_texture;
	}

	Texture* SceneObject::GetTexture()
	{
		return m_texture;
	}

	IPhysicsBody* SceneObject::GetPhysicsBody()
	{
		return m_physicsBody;
	}
}
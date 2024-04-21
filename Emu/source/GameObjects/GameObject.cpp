#pragma once

#include "../../include/GameObjects/GameObject.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/BodyTypes.h"
#include "../../include/Textures/Texture.h"

namespace Engine
{
	GameObject::GameObject(IPhysicsBody* ptrPhysicsBody, Texture* ptrTexture) : m_physicsBody(ptrPhysicsBody), m_texture(ptrTexture)
	{
		ENGINE_INFO_D("GameObject created");
	}

	GameObject::~GameObject()
	{
		ENGINE_INFO_D("Freeing GameObject!");
		if (m_physicsBody != nullptr)
			delete m_physicsBody;

		//delete m_texture;
	}

	Texture* GameObject::GetTexture()
	{
		return m_texture;
	}

	IPhysicsBody* GameObject::GetPhysicsBody()
	{
		return m_physicsBody;
	}
}
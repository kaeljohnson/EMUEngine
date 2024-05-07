#pragma once

#include "../../include/ClientObjects/ClientObject.h"

ClientObject::ClientObject(Engine::IPhysicsBody* ptrPhysicsBody, Engine::Texture* ptrTexture) : Engine::SceneObject(ptrPhysicsBody, ptrTexture)
{}

ClientObject::~ClientObject()
{
	CLIENT_INFO_D("Freeing SceneObject!");
	if (m_physicsBody != nullptr)
		delete m_physicsBody;
	m_physicsBody = nullptr;
}

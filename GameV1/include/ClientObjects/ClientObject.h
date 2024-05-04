#pragma once

#include <string>
#include <Engine.h>

// Example of client defined listener. Processes events related to application management.

class ClientObject : public Engine::SceneObject
{
public:
	ClientObject(Engine::IPhysicsBody* ptrPhysicsBody, Engine::Texture* ptrTexture);
	~ClientObject();

};
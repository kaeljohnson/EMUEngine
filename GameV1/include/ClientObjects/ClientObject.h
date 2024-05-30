#pragma once

#include <memory>
#include <Engine.h>

// Example of client defined listener. Processes events related to application management.

class ClientObject : public Engine::SceneObject
{
public:
	ClientObject(std::shared_ptr<Engine::IPhysicsBody> ptrPhysicsBody, Engine::Texture& ptrTexture);
};
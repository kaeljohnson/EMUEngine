#pragma once

#include <memory>

#include "../../include/ClientObjects/ClientObject.h"

ClientObject::ClientObject(std::shared_ptr<Engine::IPhysicsBody> ptrPhysicsBody, Engine::Texture* ptrTexture) : Engine::SceneObject(ptrPhysicsBody, ptrTexture)
{}



#pragma once

#include <memory>
#include <Engine.h>

// Example of client defined listener. Processes events related to application management.

class ClientObject : public Engine::SceneObject
{
public:
	ClientObject(const Engine::BodyType bodyType, const bool fixed, const float startingXInMeters, const float startingYInMeters,
		const float widthInMeters, const float heightInMeters, Engine::TexturePtr refTexture);
};
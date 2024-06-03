#pragma once

#include <memory>

#include "../../include/ClientObjects/ClientObject.h"

ClientObject::ClientObject(const Engine::BodyType bodyType, const bool fixed, const float startingXInMeters, 
	const float startingYInMeters, const float widthInMeters, const float heightInMeters, Engine::Texture& refTexture) : 
	Engine::SceneObject(bodyType, fixed, startingXInMeters, startingYInMeters, widthInMeters, heightInMeters, refTexture)
{}




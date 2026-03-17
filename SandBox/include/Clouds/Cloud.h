#pragma once

#include <Engine.h>

class Cloud
{
public:
	Cloud();
	~Cloud() = default;

	void Update(Engine::Entity entity);
};
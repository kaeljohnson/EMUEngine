#pragma once

#include <Engine.h>

struct PlayerContactListener : public Engine::SingleEntityContactListener
{
	PlayerContactListener(Engine::Entity* ptrPlayerEntity)
		: Engine::SingleEntityContactListener(ptrPlayerEntity) {}

	void OnContactBegin(const Engine::ContactEvent event) override
	{
		CLIENT_INFO_D("Player contact Begin");
	}

	void OnContactEnd(const Engine::ContactEvent event) override
	{
		CLIENT_INFO_D("Player contact End");
	}
};

struct PlayerSensorListener : public Engine::SingleEntitySensorListener
{
	PlayerSensorListener(Engine::Entity* ptrPlayerEntity)
		: Engine::SingleEntitySensorListener(ptrPlayerEntity) {}

	void OnContactBegin(const Engine::ContactEvent event) override
	{
		CLIENT_INFO_D("Player sensor Begin");
	}

	void OnContactEnd(const Engine::ContactEvent event) override
	{
		CLIENT_INFO_D("Player sensor End");
	}
};
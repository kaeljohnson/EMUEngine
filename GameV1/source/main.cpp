#pragma once

#include <Engine.h>
#include "../include/EventHandlers/AppManagementEventHandlers.h"
#include "../include/Player/Player.h"
#include "../include/Camera/PlayerCamera.h"

#include <iostream>
#include <mutex>


static std::unordered_map<void*, size_t> allocationMap;
static std::mutex allocMutex;

static size_t totalAllocated = 0;

std::atomic<int> allocationCount = 0;

void* operator new(std::size_t size) {
	void* ptr = std::malloc(size);
	totalAllocated += size;

	int count = ++allocationCount;

	{
		std::lock_guard<std::mutex> lock(allocMutex);
		std::cout << "Allocated " << size << " bytes (count: " << count << ")\n";
	}

	if (count == 400) {
		std::lock_guard<std::mutex> lock(allocMutex);
		std::cout << "=== 400th allocation reached ===\n";
		// throw std::runtime_error("400th allocation reached");
	}

	return ptr;
}

void operator delete(void* ptr) noexcept {
	std::lock_guard<std::mutex> lock(allocMutex);
	std::cout << "Freed bytes from " << ptr << "\n";
	free(ptr);
}

int main(int argc, char* args[])
{
	Engine::Init(10000);
	Engine::EMU* engine = Engine::EMU::GetInstance();

	CLIENT_INFO_D("Client Running!");

	Engine::Scene& scene = engine->CreateScene("Level1");
	Engine::Scene& scene2 = engine->CreateScene("Level2");

	//// Need physcis to scale with pixels per unit.
	scene.SetPhysicsSimulation(Engine::Vector2D(0.0f, 0.0f));
	scene2.SetPhysicsSimulation(Engine::Vector2D(0.0f, 100.0f));

	// Temp for now. Client will not be able to call functions directly on scene object.
	std::vector<std::pair<Engine::Entity, char>>& tileMap = scene.AddTileMap("testMap1.txt", "Rules.json");
	scene2.AddTileMap("TestMap2.txt", "Rules.json");

	Player player;

	Engine::Entity playerEntityScene1 = Engine::EMU::GetInstance()->GetEntityByCharacter('P', "Level1");
	Engine::Entity playerEntityScene2 = Engine::EMU::GetInstance()->GetEntityByCharacter('P', "Level2");
	Engine::Entity testEntity = Engine::EMU::GetInstance()->GetEntityByCharacter('S', "Level1");

	scene.RegisterContactCallback(Engine::BEGIN_CONTACT, playerEntityScene1, testEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Multi Begin Contact");
		});

	scene.RegisterContactCallback(Engine::END_CONTACT, playerEntityScene1, testEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Multi End Contact");
		});

	scene.RegisterContactCallback(Engine::BEGIN_SENSOR, testEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Single Begin Sensing");
		});

	scene.RegisterContactCallback(Engine::BEGIN_SENSOR, playerEntityScene1, testEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Single Begin Sensing");
		});

	scene.RegisterContactCallback(Engine::END_SENSOR, testEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Single End Sensing");
		});
	scene.RegisterContactCallback(Engine::END_SENSOR, playerEntityScene1, testEntity, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Single End Sensing");
		});

	PlayerCamera playerCamera;
	// engine->SetCurrentCamera(ptrCameraEntity);
	// scene->Add(ptrCameraEntity);

	// scene2.Add(playerEntity);


	engine->LoadScene("Level1");
	
	AppManagementEventHandlers appManagementEventHandlers;

	engine->RunApp();
	// Need to figure out how to change scenes, stop scenes, etc.
	 
	CLIENT_INFO("Total Allocated bytes " + std::to_string(totalAllocated));

	return 0;
}
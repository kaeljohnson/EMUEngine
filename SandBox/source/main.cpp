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

//void* operator new(std::size_t size) {
//	void* ptr = std::malloc(size);
//	totalAllocated += size;
//
//	int count = ++allocationCount;
//
//	{
//		std::lock_guard<std::mutex> lock(allocMutex);
//		std::cout << "Allocated " << size << " bytes (count: " << count << ")\n";
//	}
//
//	if (count == 400) {
//		std::lock_guard<std::mutex> lock(allocMutex);
//		std::cout << "=== 400th allocation reached ===\n";
//		// throw std::runtime_error("400th allocation reached");
//	}
//
//	return ptr;
//}
//
//void operator delete(void* ptr) noexcept {
//	std::lock_guard<std::mutex> lock(allocMutex);
//	std::cout << "Freed bytes from " << ptr << "\n";
//	free(ptr);
//}

int main(int argc, char* args[])
{
	Engine::Init(10000);

	Engine::EMU* engine = Engine::EMU::GetInstance();

	CLIENT_INFO_D("Client Running!");

	engine->Scenes_Create("Level1");
	engine->Scenes_Create("Level2");

	//// Need physcis to scale with pixels per unit.
	engine->Scenes_SetGravity("Level1", Math2D::Point2D(0.0f, 100.0f));
	engine->Scenes_SetGravity("Level2", Math2D::Point2D(0.0f, 100.0f));

	// Temp for now. Client will not be able to call functions directly on scene object.
	engine->Scenes_AddTileMap("Level1", "TestMap2.txt", "TestSceneRules.json");
	engine->Scenes_AddTileMap("Level2", "TestMap1.txt", "TestSceneRules.json");

	Player player;

	engine->Scenes_RegisterContactCallback("Level1", Engine::BEGIN_CONTACT, 1, 2, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Multi Begin Contact");
		});

	engine->Scenes_RegisterContactCallback("Level1", Engine::END_CONTACT, 1, 2, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Multi End Contact");
		});

	engine->Scenes_RegisterContactCallback("Level1", Engine::BEGIN_SENSOR, 2, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Single Begin Sensing");
		});

	engine->Scenes_RegisterContactCallback("Level1", Engine::BEGIN_SENSOR, 1, 2, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Multi Begin Sensing");
		});

	engine->Scenes_RegisterContactCallback("Level1", Engine::END_SENSOR, 2, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Single End Sensing");
		});
	engine->Scenes_RegisterContactCallback("Level1", Engine::END_SENSOR, 1, 2, [](const Engine::Contact event)
		{
			CLIENT_INFO_D("Multi End Sensing");
		});

	PlayerCamera playerCamera;

	engine->Scenes_Load("Level1");

	engine->Scenes_RegisterOnPlayEvent("Level1", []()
		{
			Engine::EMU::GetInstance()->PlaySound(1, 128, true);
		});

	engine->Scenes_RegisterOnPlayEvent("Level2", []()
		{
			Engine::EMU::GetInstance()->PlaySound(1, 128, true);
		});
	
	AppManagementEventHandlers appManagementEventHandlers;

	engine->RunApp();
	// Need to figure out how to change scenes, stop scenes, etc.
	 
	CLIENT_INFO("Total Allocated bytes " + std::to_string(totalAllocated));

	return 0;
}
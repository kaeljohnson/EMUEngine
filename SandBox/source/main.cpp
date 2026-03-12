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

//std::atomic<int> allocationCount = 0;
//
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

	engine->Scenes_Create("StartScreenRules.json", "StartScreen");
	// engine->Scenes_Create("Level1");

	Player player;

	engine->Scenes_RegisterContactCallback("StartScreen", Engine::BEGIN_SENSOR, 1, 40, [](const Engine::Contact event)
		{
			Engine::EMU::GetInstance()->Scenes_RegisterIOEventListener("StartScreen", Engine::W_KEY_DOWN, [](Engine::IOEvent& e)
				{
					Engine::EMU::GetInstance()->Scenes_Load("Level1");
					e.Handled = true;
				});
		});

	engine->Scenes_RegisterContactCallback("StartScreen", Engine::END_SENSOR, 1, 40, [](const Engine::Contact event)
		{
			Engine::EMU::GetInstance()->Scenes_UnRegisterIOEventListener("StartScreen", Engine::W_KEY_DOWN);
		});

	PlayerCamera playerCamera;

	engine->Scenes_Load("StartScreen");

	engine->Scenes_RegisterOnPlayEvent("StartScreen", []()
		{
			Engine::EMU::GetInstance()->PlaySound(1, 128, true);
		});
	
	AppManagementEventHandlers appManagementEventHandlers;

	engine->RunApp();
	// Need to figure out how to change scenes, stop scenes, etc.
	 
	CLIENT_INFO("Total Allocated bytes " + std::to_string(totalAllocated));

	return 0;
}
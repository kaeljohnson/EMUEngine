#pragma once

#include "../../include/EngineConstants.h"
#include "../../include/Scenes/Scene.h"
#include "../../include/Scenes/SceneObject.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/Box.h"
#include "../../include/Physics/PhysicsFactory.h"
#include "../../include/CallbackSystem/CallbackSystem.h"

namespace Engine
{
	Scene::Scene() : m_pixelsPerMeter(0), m_gravityX(0), m_gravityY(0), m_sceneObjects(), m_world(nullptr),
		m_eventListeners() {}

	void Scene::CheckValid()
	{
		(m_world == nullptr) ? ENGINE_CRITICAL_D("World is nullptr.") : ENGINE_INFO_D("World is valid.");
		(m_sceneObjects.Size() > MAX_OBJECTS) 
			? ENGINE_CRITICAL_D("Scene object count exceeds max scene objects.") : ENGINE_INFO_D("Scene object count is valid.");
		(m_pixelsPerMeter <= 0) ? ENGINE_CRITICAL_D("Pixels per meter is invalid.") : ENGINE_INFO_D("Pixels per meter is valid.");
	}

	void Scene::Update()
	{
		// Faster way to do this? Should only have to update objects
		// prev values if they have changed. In fact, should only update
		// objects that have changed in general

		for (auto& sceneObject : m_sceneObjects)
		{
			sceneObject->UpdatePrevPosition();

			sceneObject->Update();
		}

		m_world->Update();
	};

	void Scene::SetSimulation(const float gravityX, const float gravityY, const int pixelsPerMeter)
	{
		// What happens if this is called multiple times for one scene? Make sure nothing bad.

		m_pixelsPerMeter = pixelsPerMeter;

		m_gravityX = gravityX;
		m_gravityY = gravityY;

		ENGINE_INFO_D("Setting pixels per meter, time step, gravityX and gravityY at positions: {}, {}, {}", m_pixelsPerMeter, m_gravityX, m_gravityY);

		if (m_world)
		{
			ENGINE_INFO_D("World already set!");
			return;
		}
		
		// Need a reset function for the world which resets all objects in the world.

		m_world = CreateWorld(m_gravityX * m_pixelsPerMeter, m_gravityY * m_pixelsPerMeter, 8, 3);
		

		ENGINE_INFO_D("Client creating simulation with gravity: ({}, {})", gravityX, gravityY);
	}

	void Scene::Add(SceneObject& sceneObject)
	{
		m_sceneObjects.Push(&sceneObject);

		std::shared_ptr<Box> ptrBox = std::static_pointer_cast<Box>(sceneObject.GetPhysicsBody());

		m_world->AddBox(ptrBox);
	}

	void Scene::Remove(SceneObject& sceneObject)
	{
		// Find the scene object in the array
		m_sceneObjects.Pop(&sceneObject);

		std::shared_ptr<Box> ptrBox = std::static_pointer_cast<Box>(sceneObject.GetPhysicsBody());

		m_world->RemoveBox(ptrBox);
	}

	void Scene::AddEventListener(EventListener& eventListener)
	{
		m_eventListeners.Push(&eventListener);
	}

	void Scene::RemoveEventListener(EventListener& eventListener)
	{
		m_eventListeners.Pop(&eventListener);
	}
}
#pragma once

#include "../../include/EngineConstants.h"
#include "../../include/Scenes/Scene.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/Box.h"
#include "../../include/Physics/PhysicsFactory.h"
#include "../../include/CallbackSystem/CallbackSystem.h"

namespace Engine
{
	Scene::Scene() : m_pixelsPerMeter(0), m_gravityX(0), m_gravityY(0), m_sceneObjects(), m_world(nullptr)
	{
		ICallbackSystem::GetInstance()->NewCallback(Type::SetSimulation, [this](Data data)
			{
				SetSimulation(m_gravityX, m_gravityY, m_pixelsPerMeter);
			});
	}

	void Scene::checkValid()
	{
		(m_world == nullptr) ? ENGINE_CRITICAL_D("World is nullptr.") : ENGINE_INFO_D("World is valid.");
		(m_sceneObjects.size() > MAX_OBJECTS) 
			? ENGINE_CRITICAL_D("Scene object count exceeds max scene objects.") : ENGINE_INFO_D("Scene object count is valid.");
		(m_pixelsPerMeter <= 0) ? ENGINE_CRITICAL_D("Pixels per meter is invalid.") : ENGINE_INFO_D("Pixels per meter is valid.");
	}

	void Scene::update()
	{
		// Faster way to do this? Should only have to update objects
		// prev values if they have changed. In fact, should only update
		// objects that have changed in general

		for (auto& sceneObject : m_sceneObjects)
		{
			sceneObject->GetPhysicsBody()->updatePrevX();
			sceneObject->GetPhysicsBody()->updatePrevY();

			sceneObject->update();
		}

		m_world->update();
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

	void Scene::Add(SceneObject* sceneObject)
	{
		(sceneObject == nullptr) ? ENGINE_WARN_D("SceneObject is nullptr.") : ENGINE_WARN_D("Adding scene object to scene.");

		m_sceneObjects.push(sceneObject);

		std::shared_ptr<Box> ptrBox = std::static_pointer_cast<Box>(sceneObject->GetPhysicsBody());

		m_world->addBox(ptrBox);
	}

	void Scene::Remove(SceneObject* sceneObject)
	{
		(sceneObject == nullptr) ? ENGINE_WARN_D("SceneObject is nullptr.") : ENGINE_WARN_D("Removing scene object from scene.");

		// Find the scene object in the array
		m_sceneObjects.pop(sceneObject);

		std::shared_ptr<Box> ptrBox = std::static_pointer_cast<Box>(sceneObject->GetPhysicsBody());

		m_world->removeBox(ptrBox);
	}
}
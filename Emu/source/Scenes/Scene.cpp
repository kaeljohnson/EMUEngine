#pragma once

#include <string>

#include "../../include/Scenes/Scene.h"
#include "../../include/Logging/Logger.h"
#include "../../include/Physics/Box.h"
#include "../../include/Physics/PhysicsFactory.h"

namespace Engine
{
	Scene::Scene(std::string name, const float timestep, const int pixelsPerMeter) 
		: m_name(name), m_pixelsPerMeter(pixelsPerMeter), m_timeStep(timestep),
		m_world(CreateWorld(0.0f * m_pixelsPerMeter, 9.8f * m_pixelsPerMeter, m_timeStep, 8, 3))
	{}

	Scene::~Scene()
	{
		delete m_world;
		m_world = nullptr;
	}

	void Scene::checkValid()
	{
		(m_world == nullptr) ? ENGINE_CRITICAL_D("World is nullptr.") : ENGINE_INFO_D("World is valid.");
		(m_sceneObjects.size() > m_sceneObjects.MAX_OBJECTS) 
			? ENGINE_CRITICAL_D("Scene object count exceeds max scene objects.") : ENGINE_INFO_D("Scene object count is valid.");
		(m_pixelsPerMeter <= 0) ? ENGINE_CRITICAL_D("Pixels per meter is invalid.") : ENGINE_INFO_D("Pixels per meter is valid.");
		(m_timeStep <= 0) ? ENGINE_CRITICAL_D("Time step is invalid.") : ENGINE_INFO_D("Time step is valid.");
		(m_name.empty()) ? ENGINE_CRITICAL_D("Scene name is empty.") : ENGINE_INFO_D("Scene name is valid.");
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
		}

		m_world->update();
	};

	void Scene::SetSimulation(const float gravityX, const float gravityY)
	{
		m_world->SetGravity(gravityX * m_pixelsPerMeter, gravityY * m_pixelsPerMeter);
		m_world->SetTimeStep(m_timeStep);

		ENGINE_INFO_D("Client creating simulation with gravity: ({}, {})", gravityX, gravityY);
	}

	void Scene::Add(SceneObject* sceneObject)
	{
		(sceneObject == nullptr) ? ENGINE_WARN_D("SceneObject is nullptr.") : ENGINE_WARN_D("Adding scene object to scene {}.", m_name);

		m_sceneObjects.push(sceneObject);

		m_world->addBox(static_cast<Box*>(sceneObject->GetPhysicsBody()));
	}

	void Scene::Remove(SceneObject* sceneObject)
	{
		(sceneObject == nullptr) ? ENGINE_WARN_D("SceneObject is nullptr.") : ENGINE_WARN_D("Removing scene object from scene {}.", m_name);

		// Find the scene object in the array
		m_sceneObjects.pop(sceneObject);

		m_world->removeBox(static_cast<Box*>(sceneObject->GetPhysicsBody()));
	}
}
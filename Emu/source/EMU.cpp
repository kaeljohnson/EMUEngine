#pragma once

#include "../include/EMU.h"

namespace Engine
{
	static EMU* m_instance;

	void EMU::Init()
	{
		if (!m_instance)
			m_instance = new EMU();
	}

	EMU* EMU::GetInstance()
	{
		return m_instance;
	}

	EMU::EMU()
		: m_ecs(), m_cameraManager(), m_windowRenderer(m_ecs), m_sceneManager(), m_contactSystem(m_ecs), 
		m_physicsInterface(m_ecs), m_application(m_ecs, m_cameraManager, m_windowRenderer, m_sceneManager)
	{
		m_ecs.Initialize(10000);

		m_ecs.RegisterComponentManager<Updatable>();
		m_ecs.RegisterComponentManager<PhysicsBody>();
		m_ecs.RegisterComponentManager<Transform>();
		m_ecs.RegisterComponentManager<Camera>();
		m_ecs.RegisterComponentManager<SimpleContact>();
	}

	EMU::~EMU()
	{
		// Temo cleanup until ECS and contact system are integrated as members.
		// ContactSystem::Cleanup();
		// ECS::Cleanup();
		delete m_instance;
	}

	// Scene management
	std::shared_ptr<Scene> EMU::CreateScene(const std::string& name)
	{
		std::shared_ptr<Scene> scene = std::make_shared<Scene>(m_ecs, m_physicsInterface, m_contactSystem);
		m_sceneManager.AddScene(name, scene);
		return scene;
	}

	void EMU::LoadScene(const std::string& name)
	{
		m_sceneManager.LoadScene(name);
	}

	void EMU::UnloadCurrentScene()
	{
		m_sceneManager.UnloadCurrentScene();
	}

	/*void EMU::RemoveScene(const std::string& name)
	{

	}

	Scene* EMU::GetScene(const std::string& name)
	{

	}*/

	// Camera management
	void EMU::SetCurrentCamera(Entity* ptrCameraEntity)
	{
		m_cameraManager.SetCurrentCamera(ptrCameraEntity);
	}
}
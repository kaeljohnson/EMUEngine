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
		: m_ecs(), m_sceneManager(), 
		m_physicsInterface(m_ecs), m_transformInterface(m_ecs), m_updatableInterface(m_ecs), 
		m_application(m_ecs, m_sceneManager), m_cameraInterface(m_ecs)
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
		delete m_instance;
	}

	// Scene management
	std::shared_ptr<Scene> EMU::CreateScene(const std::string& name)
	{
		std::shared_ptr<Scene> scene = std::make_shared<Scene>(m_ecs, m_physicsInterface, m_cameraInterface);
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

	Entity* EMU::CreateEntity()
	{
		return m_ecs.CreateEntity();
	}
}
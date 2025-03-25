#pragma once

#include "../include/EMU.h"

namespace Engine
{
	static EMU* m_instance;

	void EMU::Init(const size_t numEntities)
	{
		if (!m_instance)
			m_instance = new EMU(numEntities);
	}

	EMU* EMU::GetInstance()
	{
		return m_instance;
	}

	EMU::EMU(const size_t numEntities)
		: m_ecs(), m_sceneManager(), 
		m_physicsInterface(m_ecs), m_transformInterface(m_ecs), 
		m_updatableInterface(m_ecs), m_ioEventSystem(),
		m_application(m_ecs, m_sceneManager, m_ioEventSystem), m_cameraInterface(m_ecs)
	{
		m_ecs.Initialize(numEntities);

		m_ecs.RegisterComponentManager<Updatable>();
		m_ecs.RegisterComponentManager<PhysicsBody>();
		m_ecs.RegisterComponentManager<Transform>();
		m_ecs.RegisterComponentManager<Camera>();
		m_ecs.RegisterComponentManager<SimpleContact>();

		m_ioEventSystem.Initialize();
	}

	EMU::~EMU() 
	{
		delete m_instance;
	}

	// Scene management
	std::shared_ptr<Scene> EMU::CreateScene(const std::string& name)
	{
		std::shared_ptr<Scene> scene = std::make_shared<Scene>(m_ecs);
		m_sceneManager.AddScene(name, scene);
		return scene;
	}

	void EMU::LoadScene(const std::string& name)
	{
		// m_sceneManager.LoadScene(name);
		m_sceneManager.QueueNewScene(name);
	}

	void EMU::UnloadCurrentScene()
	{
		m_sceneManager.UnloadCurrentScene();
	}

	Entity EMU::CreateEntity()
	{
		return m_ecs.CreateEntity();
	}

	void EMU::Activate(Entity entity)
	{
		m_ecs.Activate(entity);
		m_physicsInterface.ActivateBody(entity);
		m_transformInterface.Activate(entity);
		m_cameraInterface.Activate(entity);
		m_updatableInterface.Activate(entity);
	}

	void EMU::Deactivate(Entity entity)
	{
		m_ecs.Deactivate(entity);
		m_physicsInterface.DeactivateBody(entity);
		m_transformInterface.Deactivate(entity);
		m_updatableInterface.Deactivate(entity);
	}

	void EMU::RegisterIOEventListener(IOEventType type, IOEventHandler handler)
	{
		m_ioEventSystem.RegisterIOEventListener(type, handler);
	}
}
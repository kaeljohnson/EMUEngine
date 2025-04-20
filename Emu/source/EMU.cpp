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
		m_updaterInterface(m_ecs), m_ioEventSystem(),
		m_application(m_ecs, m_sceneManager, m_ioEventSystem), m_cameraInterface(m_ecs)
	{
		m_ecs.Initialize(numEntities);

		m_ecs.RegisterComponentManager<PhysicsUpdater>();
		m_ecs.RegisterComponentManager<CameraUpdater>();
		m_ecs.RegisterComponentManager<PhysicsBody>();
		m_ecs.RegisterComponentManager<Transform>();
		m_ecs.RegisterComponentManager<Camera>();
		m_ecs.RegisterComponentManager<ChainCollider>();
		m_ecs.RegisterComponentManager<ChainColliderTop>();
		m_ecs.RegisterComponentManager<ChainColliderLeft>();
		m_ecs.RegisterComponentManager<ChainColliderRight>();
		m_ecs.RegisterComponentManager<ChainColliderBottom>();

		m_ioEventSystem.Initialize();
	}

	EMU::~EMU() 
	{
		delete m_instance;
	}

	// Scene management
	Scene& EMU::CreateScene(const std::string& name)
	{
		m_sceneManager.AddScene(name, m_ecs);
		return m_sceneManager.GetScene(name);
	}

	void EMU::LoadScene(const std::string& name)
	{
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

	Entity EMU::GetEntityByCharacter(const char c, const std::string& sceneName)
	{
		return GetScene(sceneName).GetTileMapEntity(c);
	}

	std::vector<Entity> EMU::GetEntitiesByCharacter(const char c, const std::string& sceneName)
	{
		return GetScene(sceneName).GetTileMapEntities(c);
	}

	Entity EMU::GetCurrentRuntimeEntity(const char c)
	{
		return m_sceneManager.GetCurrentScene()->GetTileMapEntity(c);
	}

	void EMU::Activate(Entity entity)
	{
		m_ecs.Activate(entity);
		m_sceneManager.GetCurrentScene()->ActivatePhysics(entity);

		// maybe the activation deactivation happens in in the system and not the interface?
		m_transformInterface.Activate(entity);
		m_updaterInterface.Activate(entity);

		m_application.Activate(entity);
	}

	void EMU::ChangeCamera(Entity entity)
	{
		m_cameraInterface.ChangeCamera(entity);
	}

	void EMU::Deactivate(Entity entity)
	{
		m_ecs.Deactivate(entity);
		m_sceneManager.GetCurrentScene()->DeactivatePhysics(entity);

		// maybe the activation deactivation happens in in the system and not the interface?
		m_transformInterface.Deactivate(entity);
		m_updaterInterface.Deactivate(entity);

		m_application.Deactivate(entity);
	}

	void EMU::RegisterIOEventListener(IOEventType type, IOEventHandler handler)
	{
		m_ioEventSystem.RegisterIOEventListener(type, handler);
	}
}
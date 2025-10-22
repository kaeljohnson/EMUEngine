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
		: m_ecs(), m_sceneManager(), m_assetManager(), m_animationSystem(m_ecs), 
		m_audioSystem(m_ecs, m_assetManager), m_physicsInterface(m_ecs), m_transformInterface(m_ecs), m_ioEventSystem(),
		m_application(m_ecs, m_sceneManager, m_ioEventSystem, m_assetManager, m_audioSystem, m_animationSystem), m_cameraInterface(m_ecs)
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
		m_ecs.RegisterComponentManager<Animations>();
		m_ecs.RegisterComponentManager<AudioSource>();

		m_ioEventSystem.Initialize();
	}

	EMU::~EMU() 
	{
		delete m_instance;
	}

	// Scene management
	Scene& EMU::CreateScene(const std::string& name)
	{
		m_sceneManager.AddScene(name, m_ecs, m_assetManager);
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
		m_sceneManager.GetCurrentScene()->Activate(entity);
	}

	/*void EMU::ChangeCamera(Entity entity)
	{
		m_cameraInterface.ChangeCamera(entity);
	}*/

	void EMU::PlaySound(int soundIndex, int volume, const bool loop)
	{
		m_audioSystem.PlaySound(soundIndex, volume, loop);
	}

	void EMU::Deactivate(Entity entity)
	{
		m_sceneManager.GetCurrentScene()->Deactivate(entity);
		m_ecs.Deactivate(entity);
	}

	void EMU::RegisterIOEventListener(IOEventType type, IOEventHandler handler)
	{
		m_ioEventSystem.RegisterIOEventListener(type, handler);
	}

	void EMU::Camera_SetPixelsPerUnit(Entity entity, const int pixelsPerUnit) { m_cameraInterface.SetPixelsPerUnit(entity, pixelsPerUnit); }
	const int EMU::Camera_GetPixelsPerUnit(Entity entity) { return m_cameraInterface.GetPixelsPerUnit(entity); }
	void EMU::Camera_SetOffset(Entity entity, const Vector2D<float> offset) { m_cameraInterface.SetOffset(entity, offset); }
	const Vector2D<float> EMU::Camera_GetOffset(Entity entity) { return m_cameraInterface.GetOffset(entity); }
	void EMU::Camera_SetClampingOn(Entity entity, const bool clampingOn) { m_cameraInterface.SetClampingOn(entity, clampingOn); }
	const bool EMU::Camera_GetClampingOn(Entity entity) { return m_cameraInterface.GetClampingOn(entity); }
	const Vector2D<float> EMU::Camera_GetSize(Entity entity) { return m_cameraInterface.GetSize(entity); }
	void EMU::Camera_SetOffsets(Entity entity, const Vector2D<float> offsets) { m_cameraInterface.SetOffsets(entity, offsets); }

	// PhysicsBody2d getter and setter wrappers
	void EMU::Physics_CreateBody(Entity entity) { m_physicsInterface.CreateBody(entity); }
	const bool EMU::Physics_HasBody(Entity entity) { return m_physicsInterface.HasBody(entity); }
	void EMU::Physics_SetBodyType(Entity entity, const BodyType type) { m_physicsInterface.SetBodyType(entity, type); }
	void EMU::Physics_SetDimensions(Entity entity, const Vector2D<float> dimensions) { m_physicsInterface.SetDimensions(entity, dimensions); }
	const Vector2D<float> EMU::Physics_GetDimensions(Entity entity) { return m_physicsInterface.GetDimensions(entity); }
	const Vector2D<float> EMU::Physics_GetDimensions(PhysicsBody& body) { return m_physicsInterface.GetDimensions(body); }
	void EMU::Physics_SetGravity(Entity entity, bool enabled) { m_physicsInterface.SetGravity(entity, enabled); }
	void EMU::Physics_SetStartingPosition(Entity entity, const Vector2D<float> position) { m_physicsInterface.SetStartingPosition(entity, position); }
	void EMU::Physics_SetPosition(Entity entity, const Vector2D<float> position) { m_physicsInterface.SetPosition(entity, position); }
	const Vector2D<float> EMU::Physics_GetPosition(Entity entity) { return m_physicsInterface.GetPosition(entity); }
	const Vector2D<float> EMU::Physics_GetTopLeftPosition(Entity entity) { return m_physicsInterface.GetTopLeftPosition(entity); }
	void EMU::Physics_ApplyForceToBody(Entity entity, Vector2D<float> force) { m_physicsInterface.ApplyForceToBody(entity, force); }
	void EMU::Physics_ApplyImpulseToBody(Entity entity, Vector2D<float> impulse) { m_physicsInterface.ApplyImpulseToBody(entity, impulse); }
	void EMU::Physics_SetVelocity(Entity entity, const Vector2D<float> velocity) { m_physicsInterface.SetVelocity(entity, velocity); }
	void EMU::Physics_SetXVelocity(Entity entity, const float xVelocity) { m_physicsInterface.SetXVelocity(entity, xVelocity); }
	void EMU::Physics_SetYVelocity(Entity entity, const float yVelocity) { m_physicsInterface.SetYVelocity(entity, yVelocity); }
	void EMU::Physics_SetDeceleration(Entity entity, const float decel) { m_physicsInterface.SetDeceleration(entity, decel); }
	const Vector2D<float> EMU::Physics_GetVelocity(Entity entity) { return m_physicsInterface.GetVelocity(entity); }
	void EMU::Physics_SetRestitution(Entity entity, const float restitution) { m_physicsInterface.SetRestitution(entity, restitution); }
	void EMU::Physics_SetDensity(Entity entity, const float density) { m_physicsInterface.SetDensity(entity, density); }
	void EMU::Physics_SetFriction(Entity entity, const float friction) { m_physicsInterface.SetFriction(entity, friction); }
	void EMU::Physics_SetFixedRotation(Entity entity, bool fixed) { m_physicsInterface.SetFixedRotation(entity, fixed); }
	const float EMU::Physics_GetAngleInRadians(Entity entity) { return m_physicsInterface.GetAngleInRadians(entity); }
	const float EMU::Physics_GetAngleInDegrees(Entity entity) { return m_physicsInterface.GetAngleInDegrees(entity); }
	void EMU::Physics_RemoveBodyFromWorld(Entity entity) { m_physicsInterface.RemoveBodyFromWorld(entity); }

	// Contact System interface
	const bool EMU::Physics_HasContactBelow(Entity entity) { return m_physicsInterface.HasContactBelow(entity); }
	const bool EMU::Physics_HasContactAbove(Entity entity) { return m_physicsInterface.HasContactAbove(entity); }
	const bool EMU::Physics_HasContactLeft(Entity entity) { return m_physicsInterface.HasContactLeft(entity); }
	const bool EMU::Physics_HasContactRight(Entity entity) { return m_physicsInterface.HasContactRight(entity); }
}
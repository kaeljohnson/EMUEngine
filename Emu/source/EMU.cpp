#pragma once

#include "../include/EMU.h"
#include "../include/Rendering/Screen.h"

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
		: m_ecs(), m_sceneManager(m_ecs), m_assetManager(), m_animationSystem(m_ecs), 
		m_audioSystem(m_ecs, m_assetManager), m_physicsInterface(m_ecs), m_transformInterface(m_ecs), m_ioEventSystem(),
		m_application(m_ecs, m_sceneManager, m_ioEventSystem, m_assetManager, m_audioSystem, m_animationSystem), m_cameraInterface(m_ecs)
	{
		m_ecs.Initialize(numEntities);

		m_ecs.RegisterComponentManager<PhysicsUpdater>();
		m_ecs.RegisterComponentManager<CameraUpdater>();
		m_ecs.RegisterComponentManager<PhysicsBody>();
		m_ecs.RegisterComponentManager<ChainCollider>();
		m_ecs.RegisterComponentManager<Transform>();
		m_ecs.RegisterComponentManager<Camera>();
		m_ecs.RegisterComponentManager<Sprite>();
		m_ecs.RegisterComponentManager<Animations>();
		m_ecs.RegisterComponentManager<AudioSource>();

		m_ioEventSystem.Initialize();
	}

	EMU::~EMU() 
	{
		delete m_instance;
	}

	Entity EMU::ECS_CreateEntity()
	{
		return m_ecs.CreateEntity();
	}

	/*void EMU::ChangeCamera(Entity entity)
	{
		m_cameraInterface.ChangeCamera(entity);
	}*/

	void EMU::PlaySound(int soundIndex, int volume, const bool loop) { m_audioSystem.PlaySound(soundIndex, volume, loop); }


	void EMU::RegisterIOEventListener(IOEventType type, IOEventHandler handler)
	{
		m_ioEventSystem.RegisterIOEventListener(type, handler);
	}

	// Scene management

	Entity EMU::Scenes_GetEntityById(const std::string& sceneName, const size_t tileId)
	{
		return m_sceneManager.GetEntity(sceneName, tileId);
	}

	const std::vector<Entity>& EMU::Scenes_GetEntitiesById(const std::string& sceneName, const size_t id)
	{
		return m_sceneManager.GetEntities(sceneName, id);
	}

	Entity EMU::Scenes_GetCurrentRuntimeEntity(const size_t tileId)
	{
		return m_sceneManager.GetCurrentScene()->GetTileMapEntity(tileId);
	}

	void EMU::Scenes_Activate(Entity entity) { m_sceneManager.GetCurrentScene()->Activate(entity); }
	void EMU::Scenes_Deactivate(Entity entity) { m_sceneManager.GetCurrentScene()->Deactivate(entity); }
	void EMU::Scenes_Create(const std::string& name) { m_sceneManager.AddScene(name, m_assetManager); }
	void EMU::Scenes_Load(const std::string& name) { m_sceneManager.QueueNewScene(name); }
	void EMU::Scenes_RegisterOnPlay(const std::string& name, std::function<void()> func) { m_sceneManager.RegisterOnScenePlay(name, func); }
	void EMU::Scenes_RegisterContactCallback(const std::string& name, ContactType contactType, const size_t entityA, const size_t entityB, ContactCallback callback)
	{ 
		m_sceneManager.RegisterContactCallback(name, contactType, entityA, entityB, callback); 
	}
	void EMU::Scenes_RegisterContactCallback(const std::string& name, ContactType contactType, const size_t entity, ContactCallback callback) {
		m_sceneManager.RegisterContactCallback(name, contactType, entity, callback);
	}
	void EMU::Scenes_SetGravity(const std::string& name, const Math2D::Point2D<float> gravity) { m_sceneManager.SetGravity(name, gravity); }
	// void EMU::Scenes_Add(const std::string& name, Entity entity) { getScene(name).Add(entity); }
	// void EMU::Scenes_Remove(const std::string& name, Entity entity) { getScene(name).Remove(entity); m_ecs.Deactivate(entity); }
	void EMU::Scenes_AddTileMap(const std::string& name, const std::string& mapFileName, const std::string& rulesFileName) { m_sceneManager.AddTileMap(name, mapFileName, rulesFileName); }
	const Entity EMU::Scenes_GetTileMapEntity(const std::string& name, const size_t tileId) { return m_sceneManager.GetEntity(name, tileId); }
	const std::vector<Entity>& EMU::Scenes_GetTileMapEntities(const std::string& name, const size_t tileId) { return m_sceneManager.GetTileMapEntities(name, tileId); }
	void EMU::Scenes_SetLevelDimensions(const std::string& name, const Math2D::Point2D<int> levelWidthInUnits) { m_sceneManager.SetLevelDimensions(name, levelWidthInUnits); }

	void EMU::Camera_SetPixelsPerUnit(Entity entity, const int pixelsPerUnit) { m_cameraInterface.SetPixelsPerUnit(entity, pixelsPerUnit); }
	const size_t EMU::Camera_GetPixelsPerUnit(Entity entity) { return m_cameraInterface.GetPixelsPerUnit(entity); }
	void EMU::Camera_SetOffset(Entity entity, const Math2D::Point2D<float> offset) { m_cameraInterface.SetOffset(entity, offset); }
	const Math2D::Point2D<float> EMU::Camera_GetOffset(Entity entity) { return m_cameraInterface.GetOffset(entity); }
	void EMU::Camera_SetClampingOn(Entity entity, const bool clampingOn) { m_cameraInterface.SetClampingOn(entity, clampingOn); }
	const bool EMU::Camera_GetClampingOn(Entity entity) { return m_cameraInterface.GetClampingOn(entity); }
	const Math2D::Point2D<float> EMU::Camera_GetSize(Entity entity) { return m_cameraInterface.GetSize(entity); }
	void EMU::Camera_SetOffsets(Entity entity, const Math2D::Point2D<float> offsets) { m_cameraInterface.SetOffset(entity, offsets); }
	void EMU::Camera_AddShake(Entity entity, const Math2D::Point2D<float> intensity, const Math2D::Point2D<float> duration) { /*Add shaek to camera*/ }

	// PhysicsBody2d getter and setter wrappers
	void EMU::Physics_CreateBody(Entity entity) { m_physicsInterface.CreateBody(entity); }
	const bool EMU::Physics_HasBody(Entity entity) { return m_physicsInterface.HasBody(entity); }
	void EMU::Physics_SetBodyType(Entity entity, const BodyType type) { m_physicsInterface.SetBodyType(entity, type); }
	void EMU::Physics_SetDimensions(Entity entity, const Math2D::Point2D<float> dimensions) { m_physicsInterface.SetDimensions(entity, dimensions); }
	const Math2D::Point2D<float> EMU::Physics_GetDimensions(Entity entity) { return m_physicsInterface.GetDimensions(entity); }
	const Math2D::Point2D<float> EMU::Physics_GetDimensions(PhysicsBody& body) { return m_physicsInterface.GetDimensions(body); }
	void EMU::Physics_SetGravity(Entity entity, bool enabled) { m_physicsInterface.SetGravity(entity, enabled); }
	void EMU::Physics_SetStartingPosition(Entity entity, const Math2D::Point2D<float> position) { m_physicsInterface.SetStartingPosition(entity, position); }
	void EMU::Physics_SetPosition(Entity entity, const Math2D::Point2D<float> position) { m_physicsInterface.SetPosition(entity, position); }
	const Math2D::Point2D<float> EMU::Physics_GetPosition(Entity entity) { return m_physicsInterface.GetPosition(entity); }
	const Math2D::Point2D<float> EMU::Physics_GetTopLeftPosition(Entity entity) { return m_physicsInterface.GetTopLeftPosition(entity); }
	void EMU::Physics_ApplyForceToBody(Entity entity, Math2D::Point2D<float> force) { m_physicsInterface.ApplyForceToBody(entity, force); }
	void EMU::Physics_ApplyImpulseToBody(Entity entity, Math2D::Point2D<float> impulse) { m_physicsInterface.ApplyImpulseToBody(entity, impulse); }
	void EMU::Physics_SetVelocity(Entity entity, const Math2D::Point2D<float> velocity) { m_physicsInterface.SetVelocity(entity, velocity); }
	void EMU::Physics_SetXVelocity(Entity entity, const float xVelocity) { m_physicsInterface.SetXVelocity(entity, xVelocity); }
	void EMU::Physics_SetYVelocity(Entity entity, const float yVelocity) { m_physicsInterface.SetYVelocity(entity, yVelocity); }
	void EMU::Physics_SetDeceleration(Entity entity, const float decel) { m_physicsInterface.SetDeceleration(entity, decel); }
	const Math2D::Point2D<float> EMU::Physics_GetVelocity(Entity entity) { return m_physicsInterface.GetVelocity(entity); }
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

	// Transform getter and setter wrappers
	const Math2D::Point2D<float> EMU::Transform_GetPrevPosition(Entity entity) { return m_transformInterface.GetPrevPosition(entity); }
	const Math2D::Point2D<float> EMU::Transform_GetPrevPosition(Transform& transform) { return m_transformInterface.GetPrevPosition(transform); }
	void EMU::Transform_SetPosition(Entity entity, const Math2D::Point2D<float> position) { m_transformInterface.SetPosition(entity, position); }
	const Math2D::Point2D<float> EMU::Transform_GetPosition(Entity entity) { return m_transformInterface.GetPosition(entity); }
	void EMU::Transform_SetZIndex(Entity entity, const int zIndex) { m_transformInterface.SetZIndex(entity, zIndex); }
	const size_t EMU::Transform_GetZIndex(Entity entity) { return m_transformInterface.GetZIndex(entity); }
	void EMU::Transform_SetRotation(Entity entity, const float rotation) { m_transformInterface.SetRotation(entity, rotation); }
	const float EMU::Transform_GetRotation(Entity entity) { return m_transformInterface.GetRotation(entity); }
	void EMU::Transform_SetScale(Entity entity, const float scale) { m_transformInterface.SetScale(entity, scale); }
	const float EMU::Transform_GetScale(Entity entity) { return m_transformInterface.GetScale(entity); }
	void EMU::Transform_SetDirectionFacing(Entity entity, const int direction) { m_transformInterface.SetDirectionFacing(entity, direction); }
	const int EMU::Transform_GetDirectionFacing(Entity entity) { return m_transformInterface.GetDirectionFacing(entity); }

	// Screen interface
	const Math2D::Point2D<int> EMU::GetScreenSize() { return Screen::GetScreenSize(); }
	const Math2D::Point2D<int> EMU::GetVirtualSize() { return Screen::GetVirtualSize(); }
	const Math2D::Point2D<float> EMU::GetScale() { return Screen::GetScale(); }
	const float EMU::GetScaleConstant() { return Screen::GetScaleConstant(); }
	const Math2D::Point2D<int> EMU::GetViewportSize() { return Screen::GetViewportSize(); }
	const Math2D::Point2D<int> EMU::GetViewportPosition() { return Screen::GetViewportPosition(); }
	void EMU::SetWindowSize(const Math2D::Point2D<int>& size) { Screen::SetWindowSize(size); }
	void EMU::SetFullscreen() { Screen::SetFullscreen(); }
}
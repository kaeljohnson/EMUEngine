#pragma once

#include "Core.h"
#include "Application.h"
#include "Scenes/SceneManager.h"
#include "Events/IOEventSystem.h"
#include "Physics/Physics.h"
#include "Camera/CameraInterface.h"
#include "Audio/AudioSystem.h"
#include "TransformInterface.h"
#include "AssetManager.h"
#include "Animations/AnimationSystem.h"

namespace Engine
{
	// Need to hide all dependencies from client
	class EMU
	{
	public:
		EMU_API static EMU* GetInstance();
		EMU_API inline void RunApp() { m_application.Start(); }
		EMU_API inline void EndApp() { m_application.End(); }

		// Scene Management
		EMU_API Scene& CreateScene(const std::string& name);
		EMU_API Scene& GetScene(const std::string& name) { return m_sceneManager.GetScene(name); }
		EMU_API void LoadScene(const std::string& name);
		EMU_API void UnloadCurrentScene();

		EMU_API TransformInterface& ITRANSFORMS() { return m_transformInterface; }

		// ECS Interface functions
		EMU_API Entity CreateEntity();

		// Returns first entity with the given character.
		EMU_API Entity GetEntityByCharacter(const char c, const std::string& sceneName);

		// Returns vector of entities with the given character.
		EMU_API std::vector<Entity> GetEntitiesByCharacter(const char c, const std::string& sceneName);

		// Returns the current runtime entity with the given character.
		EMU_API Entity GetCurrentRuntimeEntity(const char c);

		// Activate all components in entity
		EMU_API void Activate(Entity entity);
		
		// Deactivate all components in entity
		EMU_API void Deactivate(Entity entity);

		// Play a sound once. Take string temporarily as they may cause dynamic allocations.
		EMU_API void PlaySound(int soundIndex, int volume, const bool loop = false);

		//////////// Camera Interface Functions ////////////

		/* 
			Sets the pixels per unit for the camera component of the given entity.
			This determines how many pixels correspond to one unit in the physicsl game world.
			arg1: entity - The entity whose camera component's pixels per unit is to be set.
			arg2: pixelsPerUnit - The number of pixels that correspond to one unit in the game world.
		*/
		EMU_API void Camera_SetPixelsPerUnit(Entity entity, const int pixelsPerUnit);

		/* 
			Gets the pixels per unit for the camera component of the given entity.
			arg1: entity - The entity whose camera component's pixels per unit is to be retrieved.
			returns: The number of pixels that correspond to one unit in the game world.
		*/
		EMU_API const int Camera_GetPixelsPerUnit(Entity entity);

		/*
			Sets the offset for the camera component of the given entity in world units.
			arg1: entity - The entity whose camera component's offset is to be set.
			arg2: offset - The offset vector in world units.
		*/
		EMU_API void Camera_SetOffset(Entity entity, const Vector2D<float> offset);

		/*
			Gets the offset for the camera component of the given entity in world units.
			arg1: entity - The entity whose camera component's offset is to be retrieved.
			returns: The offset vector in world units.
		*/
		EMU_API const Vector2D<float> Camera_GetOffset(Entity entity);

		/*
			Enables or disables clamping for the camera component of the given entity.
			With clamping enabled, the camera will not stray outside the bounds of the world.
			arg1: entity - The entity whose camera component's clamping setting is to be modified.
			arg2: clampingOn - A boolean indicating whether clamping should be enabled (true) or disabled (false).
		*/
		EMU_API void Camera_SetClampingOn(Entity entity, const bool clampingOn);

		/*
			Checks if clamping is enabled for the camera component of the given entity.
			arg1: entity - The entity whose camera component's clamping setting is to be checked.
			returns: A boolean indicating whether clamping is enabled (true) or disabled (false).
		*/
		EMU_API const bool Camera_GetClampingOn(Entity entity);

		/*
			Gets the size of camera in world units.
			arg1: entity - The entity whose camera component's size is to be retrieved.
			returns: The size of the camera in world units as a Vector2D<float>.
		*/
		EMU_API const Vector2D<float> Camera_GetSize(Entity entity);

		/* 
			Sets the offsets for the camera component of the given entity in world units.
			arg1: entity - The entity whose camera component's offsets are to be set.
			arg2: offsets - The offsets vector in world units.
		*/
		EMU_API void Camera_SetOffsets(Entity entity, const Vector2D<float> offsets);

		////////////////////////////////////////////////////

		/////////// Physics Interface Functions ////////////
		
		// PhysicsBody2d getter and setter wrappers

		/*
			Creates a physics body for the given entity.
			arg1: entity - The entity for which to create the physics body.
		*/
		EMU_API void Physics_CreateBody(Entity entity);

		/*
			Checks if the given entity has a physics body.
			arg1: entity - The entity to check for a physics body.
			returns: True if the entity has a physics body, false otherwise.
		*/
		EMU_API const bool Physics_HasBody(Entity entity);

		/*
			Sets the body type for the physics body of the given entity.
			arg1: entity - The entity whose physics body type is to be set.
			arg2: type - The BodyType to set for the physics body.
		*/
		EMU_API void Physics_SetBodyType(Entity entity, const BodyType type);

		/*
			Sets the dimensions for the physics body of the given entity.
			arg1: entity - The entity whose physics body dimensions are to be set.
			arg2: dimensions - The dimensions to set for the physics body.
		*/
		EMU_API void Physics_SetDimensions(Entity entity, const Vector2D<float> dimensions);

		/*
			Gets the dimensions for the physics body of the given entity.
			arg1: entity - The entity whose physics body dimensions are to be retrieved.
			returns: The dimensions of the physics body as a Vector2D<float>.
		*/
		EMU_API const Vector2D<float> Physics_GetDimensions(Entity entity);

		/*
			Gets the dimensions for the given physics body.
			arg1: body - The PhysicsBody whose dimensions are to be retrieved.
			returns: The dimensions of the physics body as a Vector2D<float>.
		*/
		EMU_API const Vector2D<float> Physics_GetDimensions(PhysicsBody& body);

		/*
			Enables or disables gravity for the physics body of the given entity.
			arg1: entity - The entity whose physics body gravity setting is to be modified.
			arg2: enabled - A boolean indicating whether gravity should be enabled (true) or disabled (false).
		*/
		EMU_API void Physics_SetGravity(Entity entity, bool enabled);

		/*
			Sets the starting position for the physics body of the given entity.
			arg1: entity - The entity whose physics body starting position is to be set.
			arg2: position - The starting position to set for the physics body.
		*/
		EMU_API void Physics_SetStartingPosition(Entity entity, const Vector2D<float> position);

		/*
			Sets the position for the physics body of the given entity.
			arg1: entity - The entity whose physics body position is to be set.
			arg2: position - The position to set for the physics body.
		*/
		EMU_API void Physics_SetPosition(Entity entity, const Vector2D<float> position);

		/*
			Gets the position for the physics body of the given entity.
			arg1: entity - The entity whose physics body position is to be retrieved.
			returns: The position of the physics body as a Vector2D<float>.
		*/
		EMU_API const Vector2D<float> Physics_GetPosition(Entity entity);

		/*
			Gets the top-left position for the physics body of the given entity.
			arg1: entity - The entity whose physics body top-left position is to be retrieved.
			returns: The top-left position of the physics body as a Vector2D<float>.
		*/
		EMU_API const Vector2D<float> Physics_GetTopLeftPosition(Entity entity);

		/*
			Applies force to a physics body of the given entity.
			arg1: entity - The entity whose physics body to apply force to.
			arg2: force - The force vector to apply to the physics body.
		*/
		EMU_API void Physics_ApplyForceToBody(Entity entity, Vector2D<float> force);

		/*
			Applies impulse to a physics body of the given entity.
			arg1: entity - The entity whose physics body to apply impulse to.
			arg2: impulse - The impulse vector to apply to the physics body.
		*/
		EMU_API void Physics_ApplyImpulseToBody(Entity entity, Vector2D<float> impulse);

		/*
			Sets velocity for the physics body of the given entity.
			arg1: entity - The entity whose physics body velocity is to be set.
			arg2: velocity - The velocity vector to set for the physics body.
		*/
		EMU_API void Physics_SetVelocity(Entity entity, const Vector2D<float> velocity);

		/*
			Sets the X velocity for the physics body of the given entity.
			arg1: entity - The entity whose physics body X velocity is to be set.
			arg2: xVelocity - The X velocity to set for the physics body.
		*/
		EMU_API void Physics_SetXVelocity(Entity entity, const float xVelocity);

		/*
			Sets the Y velocity for the physics body of the given entity.
			arg1: entity - The entity whose physics body Y velocity is to be set.
			arg2: yVelocity - The Y velocity to set for the physics body.
		*/
		EMU_API void Physics_SetYVelocity(Entity entity, const float yVelocity);

		/*
			Sets the deceleration for the physics body of the given entity.
			arg1: entity - The entity whose physics body deceleration is to be set.
			arg2: decel - The deceleration value to set for the physics body.
		*/
		EMU_API void Physics_SetDeceleration(Entity entity, const float decel);

		/*
			Gets the velocity for the physics body of the given entity.
			arg1: entity - The entity whose physics body velocity is to be retrieved.
			returns: The velocity of the physics body as a Vector2D<float>.
		*/
		EMU_API const Vector2D<float> Physics_GetVelocity(Entity entity);

		/*
			Sets the restitution for the physics body of the given entity.
			arg1: entity - The entity whose physics body restitution is to be set.
			arg2: restitution - The restitution value to set for the physics body.
		*/
		EMU_API void Physics_SetRestitution(Entity entity, const float restitution);

		/*
			Sets the density for the physics body of the given entity.
			arg1: entity - The entity whose physics body density is to be set.
			arg2: density - The density value to set for the physics body.
		*/
		EMU_API void Physics_SetDensity(Entity entity, const float density);

		/*
			Sets the friction for the physics body of the given entity.
			arg1: entity - The entity whose physics body friction is to be set.
			arg2: friction - The friction value to set for the physics body.
		*/
		EMU_API void Physics_SetFriction(Entity entity, const float friction);

		/*
			Sets whether the physics body of the given entity has fixed rotation.
			arg1: entity - The entity whose physics body fixed rotation setting is to be modified.
			arg2: fixed - A boolean indicating whether fixed rotation should be enabled (true) or disabled (false).
		*/
		EMU_API void Physics_SetFixedRotation(Entity entity, bool fixed);

		/*
			Gets the angle of the physics body of the given entity in radians.
			arg1: entity - The entity whose physics body angle is to be retrieved.
			returns: The angle of the physics body in radians as a float.
		*/
		EMU_API const float Physics_GetAngleInRadians(Entity entity);

		/*
			Gets the angle of the physics body of the given entity in degrees.
			arg1: entity - The entity whose physics body angle is to be retrieved.
			returns: The angle of the physics body in degrees as a float.
		*/
		EMU_API const float Physics_GetAngleInDegrees(Entity entity);

		/*
			Removes the physics body from the world for the given entity.
			arg1: entity - The entity whose physics body is to be removed from the world.
		*/
		EMU_API void Physics_RemoveBodyFromWorld(Entity entity);

		// Contact System interface

		/*
			Has contact below the physics body of the given entity.
			arg1: entity - The entity whose physics body contact below is to be checked.
		*/
		EMU_API const bool Physics_HasContactBelow(Entity entity);

		/*
			Has contact above the physics body of the given entity.
			arg1: entity - The entity whose physics body contact above is to be checked.
		*/
		EMU_API const bool Physics_HasContactAbove(Entity entity);

		/*
			Has contact to the left of the physics body of the given entity.
			arg1: entity - The entity whose physics body contact left is to be checked.
		*/
		EMU_API const bool Physics_HasContactLeft(Entity entity);

		/*
			Has contact to the right of the physics body of the given entity.
			arg1: entity - The entity whose physics body contact right is to be checked.
		*/
		EMU_API const bool Physics_HasContactRight(Entity entity);
		
		////////////////////////////////////////////////////

		template<typename T, typename... Args>
		void AddComponent(Entity entity, Args&&... componentArgs)
		{
			m_ecs.AddComponent<T>(entity, std::forward<Args>(componentArgs)...);
		}

		template <typename T, typename... Args>
		void AddComponent(const char c, Args&&... componentArgs)
		{
			std::unordered_map<std::string, Scene>& scenes = m_sceneManager.GetAllScenes();
			for (auto& scene : scenes)
			{
				std::vector<Entity> entities = scene.second.GetTileMapEntities(c);
				for (Entity entity : entities)
				{
					m_ecs.AddComponent<T>(entity, std::forward<Args>(componentArgs)...);
				}
			}
		}

		// Event IO System Interface functions
		EMU_API void RegisterIOEventListener(IOEventType type, IOEventHandler handler);

	public:
		static void Init(const size_t numEntities);
		~EMU();
		
	private:
		EMU(const size_t numEntities);

		ECS m_ecs;
		AssetManager m_assetManager;
		AnimationSystem m_animationSystem;

		AudioSystem m_audioSystem;

		PhysicsInterface m_physicsInterface;
		CameraInterface m_cameraInterface;
		TransformInterface m_transformInterface;

		IOEventSystem m_ioEventSystem;
		SceneManager m_sceneManager;
		Application m_application;
	};

} // namespace Engine
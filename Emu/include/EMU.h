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

		EMU_API PhysicsInterface& IPHYSICS() { return m_physicsInterface; }
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
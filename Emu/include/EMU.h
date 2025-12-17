#pragma once

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
		static EMU* GetInstance();
		inline void RunApp() { m_application.Start(); }
		inline void EndApp() { m_application.End(); }

		//////////// Scene Management functions ////////////

		using ContactCallback = std::function<void(const Contact&)>;

		/*
			Create a new scene with the given name.
			arg1: name - The name of the scene to be created.
		*/
		void Scenes_Create(const std::string& name);

		/*
			Load the scene with the given name. Calling this ends the current scene
			and unloads it before loading the new scene.
			arg1: name - The name of the scene to be loaded.
		*/
		void Scenes_Load(const std::string& name);

		/*
			Registers a callback function that is triggered when a new scene is played.
			arg1: name - The name of the scene for which the callback is to be registered.
			arg2: callback - The function to be called when the scene is played.
		*/
		void Scenes_RegisterOnPlay(const std::string& name, std::function<void()>);

		/*
			Register a contact callback between two entities identified by their characters.
			Call this function to add a behavior that should be triggered when two entities
			come into contact.
			arg1: name - The name of the scene where the contact callback is to be registered.
			arg2: contactType - The type of contact event (e.g., BEGIN_CONTACT, END_CONTACT).
			arg3: entityA - The character representing the first entity involved in the contact.
			arg4: entityB - The character representing the second entity involved in the contact.
		*/
		void Scenes_RegisterContactCallback(const std::string& name, ContactType contactType, const char entityA, const char entityB, ContactCallback callback);

		/*
			Register a contact callback for a single entity identified by its character.
			Call this function to add a behavior that should be triggered when the specified
			entity comes into contact with any other entity.
			arg1: name - The name of the scene where the contact callback is to be registered.
			arg2: contactType - The type of contact event (e.g., BEGIN_SENSOR, END_SENSOR).
			arg3: entity - The character representing the entity involved in the contact.
		*/
		void Scenes_RegisterContactCallback(const std::string& name, ContactType contactType, const char entity, ContactCallback callback);

		/*
			Sets the physics simulation parameters for the scene with the given name.
			arg1: name - The name of the scene for which the physics simulation is to be set.
			arg2: gravity - The gravity vector to be applied in the physics simulation.
		*/
		void Scenes_SetGravity(const std::string& name, const Math2D::Point2D<float> gravity);

		/*
			TODO: Not supported for now.
			Adds an entity to the scene with the given name.
			If this is called during runtime, the entity must be manually activated by client.
			arg1: name - The name of the scene to which the entity is to be added.
			arg2: entity - The entity to be added to the scene.
		*/
		// EMU_API void Scenes_Add(const std::string& name, Entity entity);

		/*
			Gets the first entity with the given character in the current scene.
			arg1: tileId - The tile id.
		*/
		Entity Scenes_GetCurrentRuntimeEntity(const size_t tileId);

		/*
			Gets the first entity with the given character in the specified scene.
			arg1: sceneName - The name of the scene from which to retrieve the entity.
			arg2: tileId - The tileId representing the entity to be retrieved.
		*/
		Entity Scenes_GetEntityById(const std::string& sceneName, const size_t tileId);

		/*
			Gets all entities with the given character in the specified scene.
			arg1: sceneName - The name of the scene from which to retrieve the entities.
			arg2: tileId - The number representing the entities to be retrieved.
		*/
		const std::vector<Entity>& Scenes_GetEntitiesById(const std::string& sceneName, const size_t tileId);

		/*
			Activates the specified entity in the scene with the given name.
			arg1: entity - The entity to be activated in the scene.
		*/
		void Scenes_Activate(Entity entity);

		/*
			Deactivates the specified entity in the scene with the given name.
			arg1: entity - The entity to be deactivated in the scene.
		*/
		void Scenes_Deactivate(Entity entity);

		/*
			TODO: Not supported for now.
			Removes the specified entity from the scene with the given name.
			arg1: name - The name of the scene from which the entity is to be removed.
			arg2: entity - The entity to be removed from the scene.
		*/
		// EMU_API void Scenes_Remove(const std::string& name, Entity entity);

		/*
			Adds a tile map to the scene with the given name using the specified map and rules files.
			arg1: name - The name of the scene to which the tile map is to be added.
			arg2: mapFileName - The filename of the tile map.
			arg3: rulesFileName - The filename of the rules for the tile map.
		*/
		void Scenes_AddTileMap(const std::string& name, const std::string& mapFileName, const std::string& rulesFileName);

		/*
			Gets the first entity corresponding to the specified tile character from the tile map
			of the scene with the given name.
			arg1: name - The name of the scene from which to retrieve the entity.
			arg2: tileId - The character representing the tile for which to retrieve the entity.
		*/
		const Entity Scenes_GetTileMapEntity(const std::string& name, const size_t tileId);

		/*
			Gets all entities corresponding to the specified tile character from the tile map
			of the scene with the given name.
			arg1: name - The name of the scene from which to retrieve the entities.
			arg2: tileId - The character representing the tile for which to retrieve the entities.
		*/
		const std::vector<Entity>& Scenes_GetTileMapEntities(const std::string& name, const size_t tileId);

		/*
			Sets the level dimensions for a scene without a tile map.
			arg1: name - The name of the scene for which to set the level dimensions.
			arg2: levelWidthInUnits - The dimensions of the level in world units.
		*/
		void Scenes_SetLevelDimensions(const std::string& name, const Math2D::Point2D<int> levelWidthInUnits);

		////////////////////////////////////////////////////

		TransformInterface& ITRANSFORMS() { return m_transformInterface; }

		// ECS Interface functions
		Entity CreateEntity();

		// Play a sound once. Take string temporarily as they may cause dynamic allocations.
		void PlaySound(int soundIndex, int volume, const bool loop = false);

		//////////// Camera Interface Functions ////////////

		/* 
			Sets the pixels per unit for the camera component of the given entity.
			This determines how many pixels correspond to one unit in the physicsl game world.
			arg1: entity - The entity whose camera component's pixels per unit is to be set.
			arg2: pixelsPerUnit - The number of pixels that correspond to one unit in the game world.
		*/
		void Camera_SetPixelsPerUnit(Entity entity, const int pixelsPerUnit);

		/* 
			Gets the pixels per unit for the camera component of the given entity.
			arg1: entity - The entity whose camera component's pixels per unit is to be retrieved.
			returns: The number of pixels that correspond to one unit in the game world.
		*/
		const int Camera_GetPixelsPerUnit(Entity entity);

		/*
			Sets the offset for the camera component of the given entity in world units.
			arg1: entity - The entity whose camera component's offset is to be set.
			arg2: offset - The offset vector in world units.
		*/
		void Camera_SetOffset(Entity entity, const Math2D::Point2D<float> offset);

		/*
			Gets the offset for the camera component of the given entity in world units.
			arg1: entity - The entity whose camera component's offset is to be retrieved.
			returns: The offset vector in world units.
		*/
		const Math2D::Point2D<float> Camera_GetOffset(Entity entity);

		/*
			Enables or disables clamping for the camera component of the given entity.
			With clamping enabled, the camera will not stray outside the bounds of the world.
			arg1: entity - The entity whose camera component's clamping setting is to be modified.
			arg2: clampingOn - A boolean indicating whether clamping should be enabled (true) or disabled (false).
		*/
		void Camera_SetClampingOn(Entity entity, const bool clampingOn);

		/*
			Checks if clamping is enabled for the camera component of the given entity.
			arg1: entity - The entity whose camera component's clamping setting is to be checked.
			returns: A boolean indicating whether clamping is enabled (true) or disabled (false).
		*/
		const bool Camera_GetClampingOn(Entity entity);

		/*
			Gets the size of camera in world units.
			arg1: entity - The entity whose camera component's size is to be retrieved.
			returns: The size of the camera in world units as a Math2D::Point2D<float>.
		*/
		const Math2D::Point2D<float> Camera_GetSize(Entity entity);

		/* 
			Sets the offsets for the camera component of the given entity in world units.
			arg1: entity - The entity whose camera component's offsets are to be set.
			arg2: offsets - The offsets vector in world units.
		*/
		void Camera_SetOffsets(Entity entity, const Math2D::Point2D<float> offsets);

		/*
			Add camera shake to the camera component of the given entity.
			arg1: entity - The entity whose camera component is to receive the shake effect.
			arg2: intensity - The intensity of the shake effect in world units.
			arg3: duration - The duration of the shake effect in seconds.
		*/
		void Camera_AddShake(Entity entity, const Math2D::Point2D<float> intensity, const Math2D::Point2D<float> duration);

		////////////////////////////////////////////////////

		/////////// Physics Interface Functions ////////////
		
		// PhysicsBody2d getter and setter wrappers

		/*
			Creates a physics body for the given entity.
			arg1: entity - The entity for which to create the physics body.
		*/
		void Physics_CreateBody(Entity entity);

		/*
			Checks if the given entity has a physics body.
			arg1: entity - The entity to check for a physics body.
			returns: True if the entity has a physics body, false otherwise.
		*/
		const bool Physics_HasBody(Entity entity);

		/*
			Sets the body type for the physics body of the given entity.
			arg1: entity - The entity whose physics body type is to be set.
			arg2: type - The BodyType to set for the physics body.
		*/
		void Physics_SetBodyType(Entity entity, const BodyType type);

		/*
			Sets the dimensions for the physics body of the given entity.
			arg1: entity - The entity whose physics body dimensions are to be set.
			arg2: dimensions - The dimensions to set for the physics body.
		*/
		void Physics_SetDimensions(Entity entity, const Math2D::Point2D<float> dimensions);

		/*
			Gets the dimensions for the physics body of the given entity.
			arg1: entity - The entity whose physics body dimensions are to be retrieved.
			returns: The dimensions of the physics body as a Math2D::Point2D<float>.
		*/
		const Math2D::Point2D<float> Physics_GetDimensions(Entity entity);

		/*
			Gets the dimensions for the given physics body.
			arg1: body - The PhysicsBody whose dimensions are to be retrieved.
			returns: The dimensions of the physics body as a Math2D::Point2D<float>.
		*/
		const Math2D::Point2D<float> Physics_GetDimensions(PhysicsBody& body);

		/*
			Enables or disables gravity for the physics body of the given entity.
			arg1: entity - The entity whose physics body gravity setting is to be modified.
			arg2: enabled - A boolean indicating whether gravity should be enabled (true) or disabled (false).
		*/
		void Physics_SetGravity(Entity entity, bool enabled);

		/*
			Sets the starting position for the physics body of the given entity.
			arg1: entity - The entity whose physics body starting position is to be set.
			arg2: position - The starting position to set for the physics body.
		*/
		void Physics_SetStartingPosition(Entity entity, const Math2D::Point2D<float> position);

		/*
			Sets the position for the physics body of the given entity.
			arg1: entity - The entity whose physics body position is to be set.
			arg2: position - The position to set for the physics body.
		*/
		void Physics_SetPosition(Entity entity, const Math2D::Point2D<float> position);

		/*
			Gets the position for the physics body of the given entity.
			arg1: entity - The entity whose physics body position is to be retrieved.
			returns: The position of the physics body as a Math2D::Point2D<float>.
		*/
		const Math2D::Point2D<float> Physics_GetPosition(Entity entity);

		/*
			Gets the top-left position for the physics body of the given entity.
			arg1: entity - The entity whose physics body top-left position is to be retrieved.
			returns: The top-left position of the physics body as a Math2D::Point2D<float>.
		*/
		const Math2D::Point2D<float> Physics_GetTopLeftPosition(Entity entity);

		/*
			Applies force to a physics body of the given entity.
			arg1: entity - The entity whose physics body to apply force to.
			arg2: force - The force vector to apply to the physics body.
		*/
		void Physics_ApplyForceToBody(Entity entity, Math2D::Point2D<float> force);

		/*
			Applies impulse to a physics body of the given entity.
			arg1: entity - The entity whose physics body to apply impulse to.
			arg2: impulse - The impulse vector to apply to the physics body.
		*/
		void Physics_ApplyImpulseToBody(Entity entity, Math2D::Point2D<float> impulse);

		/*
			Sets velocity for the physics body of the given entity.
			arg1: entity - The entity whose physics body velocity is to be set.
			arg2: velocity - The velocity vector to set for the physics body.
		*/
		void Physics_SetVelocity(Entity entity, const Math2D::Point2D<float> velocity);

		/*
			Sets the X velocity for the physics body of the given entity.
			arg1: entity - The entity whose physics body X velocity is to be set.
			arg2: xVelocity - The X velocity to set for the physics body.
		*/
		void Physics_SetXVelocity(Entity entity, const float xVelocity);

		/*
			Sets the Y velocity for the physics body of the given entity.
			arg1: entity - The entity whose physics body Y velocity is to be set.
			arg2: yVelocity - The Y velocity to set for the physics body.
		*/
		void Physics_SetYVelocity(Entity entity, const float yVelocity);

		/*
			Sets the deceleration for the physics body of the given entity.
			arg1: entity - The entity whose physics body deceleration is to be set.
			arg2: decel - The deceleration value to set for the physics body.
		*/
		void Physics_SetDeceleration(Entity entity, const float decel);

		/*
			Gets the velocity for the physics body of the given entity.
			arg1: entity - The entity whose physics body velocity is to be retrieved.
			returns: The velocity of the physics body as a Math2D::Point2D<float>.
		*/
		const Math2D::Point2D<float> Physics_GetVelocity(Entity entity);

		/*
			Sets the restitution for the physics body of the given entity.
			arg1: entity - The entity whose physics body restitution is to be set.
			arg2: restitution - The restitution value to set for the physics body.
		*/
		void Physics_SetRestitution(Entity entity, const float restitution);

		/*
			Sets the density for the physics body of the given entity.
			arg1: entity - The entity whose physics body density is to be set.
			arg2: density - The density value to set for the physics body.
		*/
		void Physics_SetDensity(Entity entity, const float density);

		/*
			Sets the friction for the physics body of the given entity.
			arg1: entity - The entity whose physics body friction is to be set.
			arg2: friction - The friction value to set for the physics body.
		*/
		void Physics_SetFriction(Entity entity, const float friction);

		/*
			Sets whether the physics body of the given entity has fixed rotation.
			arg1: entity - The entity whose physics body fixed rotation setting is to be modified.
			arg2: fixed - A boolean indicating whether fixed rotation should be enabled (true) or disabled (false).
		*/
		void Physics_SetFixedRotation(Entity entity, bool fixed);

		/*
			Gets the angle of the physics body of the given entity in radians.
			arg1: entity - The entity whose physics body angle is to be retrieved.
			returns: The angle of the physics body in radians as a float.
		*/
		const float Physics_GetAngleInRadians(Entity entity);

		/*
			Gets the angle of the physics body of the given entity in degrees.
			arg1: entity - The entity whose physics body angle is to be retrieved.
			returns: The angle of the physics body in degrees as a float.
		*/
		const float Physics_GetAngleInDegrees(Entity entity);

		/*
			Removes the physics body from the world for the given entity.
			arg1: entity - The entity whose physics body is to be removed from the world.
		*/
		void Physics_RemoveBodyFromWorld(Entity entity);

		// Contact System interface

		/*
			Has contact below the physics body of the given entity.
			arg1: entity - The entity whose physics body contact below is to be checked.
		*/
		const bool Physics_HasContactBelow(Entity entity);

		/*
			Has contact above the physics body of the given entity.
			arg1: entity - The entity whose physics body contact above is to be checked.
		*/
		const bool Physics_HasContactAbove(Entity entity);

		/*
			Has contact to the left of the physics body of the given entity.
			arg1: entity - The entity whose physics body contact left is to be checked.
		*/
		const bool Physics_HasContactLeft(Entity entity);

		/*
			Has contact to the right of the physics body of the given entity.
			arg1: entity - The entity whose physics body contact right is to be checked.
		*/
		const bool Physics_HasContactRight(Entity entity);
		
		////////////////////////////////////////////////////

		//////// Transform Interface functions /////////////

		/*
			Gets the previous position of the transform component of the given entity.
			arg1: entity - The entity whose transform component's previous position is to be retrieved.
			returns: The previous position of the transform as a Math2D::Point2D<float>.
		*/
		const Math2D::Point2D<float> Transform_GetPrevPosition(Entity entity);

		/*
			Gets the previous position of the given transform.
			arg1: transform - The transform whose previous position is to be retrieved.
			returns: The previous position of the transform as a Math2D::Point2D<float>.
		*/
		const Math2D::Point2D<float> Transform_GetPrevPosition(Transform& transform);

		/*
			Sets the position of the transform component of the given entity.
			Note: This has no effect if the entity has a physics body.
			arg1: entity - The entity whose transform component's position is to be set.
			arg2: position - The position to set for the transform.
		*/
		void Transform_SetPosition(Entity entity, const Math2D::Point2D<float> position);

		/*
			Gets the position of the transform component of the given entity.
			arg1: entity - The entity whose transform component's position is to be retrieved.
			returns: The position of the transform as a Math2D::Point2D<float>.
		*/
		const Math2D::Point2D<float> Transform_GetPosition(Entity entity);

		/*
			Sets the dimensions of the transform component of the given entity.
			arg1: entity - The entity whose transform component's dimensions are to be set.
			arg2: dimensions - The dimensions to set for the transform.
		*/
		void Transform_SetDimensions(Entity entity, const Math2D::Point2D<float> dimensions);

		/*
			Gets the dimensions of the transform component of the given entity.
			arg1: entity - The entity whose transform component's dimensions are to be retrieved.
			returns: The dimensions of the transform as a Math2D::Point2D<float>.
		*/
		const Math2D::Point2D<float> Transform_GetDimensions(Entity entity);

		/*
			Sets the Z index of the transform component of the given entity.
			arg1: entity - The entity whose transform component's Z index is to be set.
			arg2: zIndex - The Z index to set for the transform.
		*/
		void Transform_SetZIndex(Entity entity, const int zIndex);

		/*
			Gets the Z index of the transform component of the given entity.
			arg1: entity - The entity whose transform component's Z index is to be retrieved.
			returns: The Z index of the transform as an int.
		*/
		const int Transform_GetZIndex(Entity entity);

		/*
			Sets the rotation of the transform component of the given entity.
			arg1: entity - The entity whose transform component's rotation is to be set.
			arg2: rotation - The rotation to set for the transform in degrees.
		*/
		void Transform_SetRotation(Entity entity, const float rotation);

		/*
			Gets the rotation of the transform component of the given entity.
			arg1: entity - The entity whose transform component's rotation is to be retrieved.
			returns: The rotation of the transform in degrees as a float.
		*/
		const float Transform_GetRotation(Entity entity);

		/*
			Sets the scale of the transform component of the given entity.
			arg1: entity - The entity whose transform component's scale is to be set.
			arg2: scale - The scale to set for the transform.
		*/
		void Transform_SetScale(Entity entity, const float scale);

		/*
			Gets the scale of the transform component of the given entity.
			arg1: entity - The entity whose transform component's scale is to be retrieved.
			returns: The scale of the transform as a float.
		*/
		const float Transform_GetScale(Entity entity);

		/*
			Sets the direction facing of the transform component of the given entity.
			arg1: entity - The entity whose transform component's direction facing is to be set.
			arg2: direction - The direction to set for the transform (e.g., 1 for right, -1 for left).
		*/
		void Transform_SetDirectionFacing(Entity entity, const int direction);

		/*
			Gets the direction facing of the transform component of the given entity.
			arg1: entity - The entity whose transform component's direction facing is to be retrieved.
			returns: The direction facing of the transform as an int (e.g., 1 for right, -1 for left).
		*/
		const int Transform_GetDirectionFacing(Entity entity);

		////////////////////////////////////////////////////

		///////////// Screen Interface functions ///////////

		/*
			Gets the display resolution of the screen.
		*/
		const Math2D::Point2D<int> GetScreenSize();

		/*
			Gets the virtual size of the screen. Based on aspect ratio of render
			output size. Virtual size height is fixed at 720 to support a reasonable
			aspect ratio across different display resolutions.
		*/
		const Math2D::Point2D<int> GetVirtualSize();

		/*
			Gets the scale factors for the screen.
		*/
		const Math2D::Point2D<float> GetScale();

		/*
			Gets the scale constant for the screen.
		*/
		const float GetScaleConstant();

		/*
			Gets the current viewport size.
		*/
		const Math2D::Point2D<int> GetViewportSize();

		/*
			Gets the current viewport position.
		*/
		const Math2D::Point2D<int> GetViewportPosition();

		/*
			Sets the window size to the specified size.
			arg1: size - The desired window size as a Math2D::Point2D<int>.
		*/
		void SetWindowSize(const Math2D::Point2D<int>& size);

		/*
			Sets the window to fullscreen mode.
		*/
		void SetFullscreen();

		////////////////////////////////////////////////////

		////// Templated ECS Interface functions ///////////

		/*
			TODO: Not supported for now. Client must add components via the rules files.
			Adds a component of type T to the specified entity with the provided arguments.
		*/
		/*template<typename T, typename... Args>
		void AddComponent(Entity entity, Args&&... componentArgs)
		{
			m_ecs.AddComponent<T>(entity, std::forward<Args>(componentArgs)...);
		}*/

		/*
			TEMP: This is only exposed for the scripting components.
			Once there is a way to register scripting components via rules files,
			this will not be exposed.
			Adds a component of type T to all entities with 
			the specified character for all scenes.
		*/
		template <typename T, typename... Args>
		void Scenes_AddComponent(const std::string sceneName, const size_t tileId, Args&&... componentArgs)
		{
			m_sceneManager.AddComponent<T>(sceneName, tileId, std::forward<Args>(componentArgs)...);
		}

		////////////////////////////////////////////////////

		// Event IO System Interface functions
		void RegisterIOEventListener(IOEventType type, IOEventHandler handler);

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
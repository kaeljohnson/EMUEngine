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
	/**
	* @brief CLIENT ENTRY POINT FOR THE EMU ENGINE.
	* 
	* All access to EMU engine systems and subsystems should be done through this class instantiated as a singleton.
	*/
	class EMU
	{
	public:

		using ContactCallback = std::function<void(const Contact&)>;

		/** 
		* @brief Get the singleton instance of the EMU engine.
		* 
		* * @return A pointer to the singleton instance of the EMU engine.
		*/
		static EMU* GetInstance();

		/**
		* @brief Starts the EMU application.
		*/
		inline void RunApp() { m_application.Start(); }

		/**
		* @brief Ends the EMU application.
		*/
		inline void EndApp() { m_application.End(); }

		//////////// Scene Management functions ////////////

		/**
		* @brief Create a new scene with the given name.
		* * @param name: The name of the scene to be created.
		*/
		void Scenes_Create(const std::string& name);

		/**
		* @brief Load the scene with the given name. Calling this ends the current scene
		* and unloads it before loading the new scene.
		* 
		* * @return name: The name of the scene to be loaded.
		*/
		void Scenes_Load(const std::string& name);

		/**
		* @brief Registers a callback function that is triggered when a new scene is played.
		* 
		* * @param name: The name of the scene for which the callback is to be registered.
		* * @param callback: The function to be called when the scene is played.
		*/
		void Scenes_RegisterOnPlay(const std::string& name, std::function<void()> callback);

		/**
		* @brief Register a contact callback between two entities identified by their tileIds.
		* Call this function to add a behavior that should be triggered when two entities
		* come into contact.
		*	
		* @param name: The name of the scene where the contact callback is to be registered.
		* @param contactType: The type of contact event (e.g., BEGIN_CONTACT, END_CONTACT).
		* @param entityA: The character representing the first entity involved in the contact.
		* @param entityB: The character representing the second entity involved in the contact.
		*/
		void Scenes_RegisterContactCallback(const std::string& name, ContactType contactType, const size_t entityA, const size_t entityB, ContactCallback callback);

		/**
		* @brief Register a contact callback for a single entity identified by its character.
		* Call this function to add a behavior that should be triggered when the specified
		* entity comes into contact with any other entity.
		*	
		* @param name: The name of the scene where the contact callback is to be registered.
		* @param contactType: The type of contact event (e.g., BEGIN_SENSOR, END_SENSOR).
		* @param entity: The character representing the entity involved in the contact.
		* @param callback: The function to be called when the contact event occurs.
		*/
		void Scenes_RegisterContactCallback(const std::string& name, ContactType contactType, const size_t entity, ContactCallback callback);

		/**
		* @brief Sets the physics simulation parameters for the scene with the given name.
		* 
		* @param name: The name of the scene for which the physics simulation is to be set.
		* @param gravity: The gravity vector to be applied in the physics simulation.
		*/
		void Scenes_SetGravity(const std::string& name, const Math2D::Point2D<float> gravity);

		/**
		* @brief Adds an entity to the scene with the given name.
		* If this is called during runtime, the entity must be manually activated by client.
		*	
		* @todo Not supported for now.
		* 
		* @param name: The name of the scene to which the entity is to be added.
		* @param entity: The entity to be added to the scene.
		*/
		void Scenes_Add(const std::string& name, Entity entity) {}

		/**
		* @brief Gets the first entity with the given character in the current scene.
		* 
		* @param tileId: The tile id.
		*/
		Entity Scenes_GetCurrentRuntimeEntity(const size_t tileId);

		/**
		* @brief Gets the first entity with the given character in the specified scene.
		* 
		* @param sceneName: The name of the scene from which to retrieve the entity.
		* @param tileId: The tileId representing the entity to be retrieved.
		*/
		Entity Scenes_GetEntityById(const std::string& sceneName, const size_t tileId);

		/**
		* @brief Gets all entities with the given character in the specified scene.
		*
		* @param sceneName: The name of the scene from which to retrieve the entities.
		* @param tileId: The number representing the entities to be retrieved.
		*/
		const std::vector<Entity>& Scenes_GetEntitiesById(const std::string& sceneName, const size_t tileId);

		/**
		* @brief Activates the specified entity in the scene with the given name.
		* 
		* @param entity: The entity to be activated in the scene.
		*/
		void Scenes_Activate(Entity entity);

		/**
		* @brief Deactivates the specified entity in the scene with the given name.
		* 
		* @param : entity - The entity to be deactivated in the scene.
		*/
		void Scenes_Deactivate(Entity entity);

		/**
		* @brief Removes the specified entity from the scene with the given name.
		* 
		* @todo Not supported for now.
		* 
		* @param name: The name of the scene from which the entity is to be removed.
		* @param entity: The entity to be removed from the scene.
		*/
		void Scenes_Remove(const std::string& name, Entity entity) {}

		/**
		* @brief Adds a tile map to the scene with the given name using the specified map and rules files.
		* 
		* @param name: The name of the scene to which the tile map is to be added.
		* @param mapFileName: The filename of the tile map.
		* @param rulesFileName: The filename of the rules for the tile map.
		*/
		void Scenes_AddTileMap(const std::string& name, const std::string& mapFileName, const std::string& rulesFileName);

		/**
		* @brief Gets the first entity corresponding to the specified tile character from the tile map
		* of the scene with the given name.
		* 
		* @param name: The name of the scene from which to retrieve the entity.
		* @param tileId: The character representing the tile for which to retrieve the entity.
		*/
		const Entity Scenes_GetTileMapEntity(const std::string& name, const size_t tileId);

		/**
		* @brief Gets all entities corresponding to the specified tile ID from the tile map
		* in the scene with the given name.
		*
		* @param name The name of the scene from which to retrieve the entities.
		* @param tileId The tile ID representing the entities to retrieve.
		* @return A reference to a vector of entities matching the tile ID.
		*/
		const std::vector<Entity>& Scenes_GetTileMapEntities(const std::string& name, const size_t tileId);

		/**
		 * @brief Sets the level dimensions for a scene that does not use a tile map.
		 *
		 * @param name The name of the scene for which to set the level dimensions.
		 * @param levelWidthInUnits The dimensions of the level in world units.
		 */
		void Scenes_SetLevelDimensions(const std::string& name, const Math2D::Point2D<int> levelWidthInUnits);


		////////////////////////////////////////////////////

		// Audio Interface functions

		// Play a sound once. Take string temporarily as they may cause dynamic allocations.
		void PlaySound(int soundIndex, int volume, const bool loop = false);

		//////////// Camera Interface Functions ////////////

		/**
		* @brief Sets the pixels-per-unit value for the camera component of the given entity.
		*
		* This determines how many screen pixels correspond to one unit in the physics world.
		*
		* @param entity The entity whose camera component is being modified.
		* @param pixelsPerUnit The number of pixels per world unit.
		*/
		void Camera_SetPixelsPerUnit(Entity entity, const int pixelsPerUnit);

		/**
		 * @brief Gets the pixels-per-unit value for the camera component of the given entity.
		 *
		 * @param entity The entity whose camera component is being queried.
		 * @return The number of pixels per world unit.
		 */
		const size_t Camera_GetPixelsPerUnit(Entity entity);

		/**
		 * @brief Sets the camera offset in world units.
		 *
		 * @param entity The entity whose camera offset is to be set.
		 * @param offset The offset vector in world units.
		 */
		void Camera_SetOffset(Entity entity, const Math2D::Point2D<float> offset);

		/**
		 * @brief Gets the camera offset in world units.
		 *
		 * @param entity The entity whose camera offset is to be retrieved.
		 * @return The offset vector in world units.
		 */
		const Math2D::Point2D<float> Camera_GetOffset(Entity entity);

		/**
		 * @brief Enables or disables camera clamping.
		 *
		 * When clamping is enabled, the camera will not move outside the bounds of the world.
		 *
		 * @param entity The entity whose camera clamping setting is to be modified.
		 * @param clampingOn True to enable clamping, false to disable it.
		 */
		void Camera_SetClampingOn(Entity entity, const bool clampingOn);

		/**
		 * @brief Checks whether camera clamping is enabled.
		 *
		 * @param entity The entity whose camera clamping setting is to be queried.
		 * @return True if clamping is enabled, false otherwise.
		 */
		const bool Camera_GetClampingOn(Entity entity);

		/**
		 * @brief Gets the size of the camera in world units.
		 *
		 * @param entity The entity whose camera size is to be retrieved.
		 * @return The size of the camera in world units.
		 */
		const Math2D::Point2D<float> Camera_GetSize(Entity entity);

		/**
		 * @brief Sets additional camera offsets in world units.
		 *
		 * @param entity The entity whose camera offsets are to be set.
		 * @param offsets The offset vector in world units.
		 */
		void Camera_SetOffsets(Entity entity, const Math2D::Point2D<float> offsets);

		/**
		 * @brief Applies a camera shake effect.
		 *
		 * @param entity The entity whose camera component will receive the shake.
		 * @param intensity The intensity of the shake in world units.
		 * @param duration The duration of the shake effect in seconds.
		 */
		void Camera_AddShake(
			Entity entity,
			const Math2D::Point2D<float> intensity,
			const Math2D::Point2D<float> duration
		);

		////////////////////////////////////////////////////

		/////////// Physics Interface Functions ////////////
		
		/**
		* @brief Creates a physics body for the given entity.
		*
		* @param entity The entity for which to create the physics body.
		*/
		void Physics_CreateBody(Entity entity);

		/**
		 * @brief Checks whether the given entity has a physics body.
		 *
		 * @param entity The entity to check for a physics body.
		 * @return True if the entity has a physics body, false otherwise.
		 */
		const bool Physics_HasBody(Entity entity);

		/**
		 * @brief Sets the body type of the physics body for the given entity.
		 *
		 * @param entity The entity whose physics body type is to be set.
		 * @param type The body type to assign to the physics body.
		 */
		void Physics_SetBodyType(Entity entity, const BodyType type);

		/**
		 * @brief Sets the dimensions of the physics body for the given entity.
		 *
		 * @param entity The entity whose physics body dimensions are to be set.
		 * @param dimensions The dimensions to apply to the physics body.
		 */
		void Physics_SetDimensions(Entity entity, const Math2D::Point2D<float> dimensions);

		/**
		 * @brief Gets the dimensions of the physics body for the given entity.
		 *
		 * @param entity The entity whose physics body dimensions are to be retrieved.
		 * @return The dimensions of the physics body.
		 */
		const Math2D::Point2D<float> Physics_GetDimensions(Entity entity);

		/**
		 * @brief Gets the dimensions of the specified physics body.
		 *
		 * @param body The physics body whose dimensions are to be retrieved.
		 * @return The dimensions of the physics body.
		 */
		const Math2D::Point2D<float> Physics_GetDimensions(PhysicsBody& body);

		/**
		 * @brief Enables or disables gravity for the physics body of the given entity.
		 *
		 * @param entity The entity whose physics body gravity setting is to be modified.
		 * @param enabled True to enable gravity, false to disable it.
		 */
		void Physics_SetGravity(Entity entity, bool enabled);

		/**
		 * @brief Sets the starting position of the physics body for the given entity.
		 *
		 * This is typically used during initialization before simulation begins.
		 *
		 * @param entity The entity whose physics body starting position is to be set.
		 * @param position The starting position in world units.
		 */
		void Physics_SetStartingPosition(Entity entity, const Math2D::Point2D<float> position);

		/**
		 * @brief Sets the current position of the physics body for the given entity.
		 *
		 * @param entity The entity whose physics body position is to be set.
		 * @param position The new position in world units.
		 */
		void Physics_SetPosition(Entity entity, const Math2D::Point2D<float> position);

		/**
		 * @brief Gets the current position of the physics body for the given entity.
		 *
		 * @param entity The entity whose physics body position is to be retrieved.
		 * @return The position of the physics body in world units.
		 */
		const Math2D::Point2D<float> Physics_GetPosition(Entity entity);

		/**
		 * @brief Gets the top-left position of the physics body for the given entity.
		 *
		 * @param entity The entity whose physics body top-left position is to be retrieved.
		 * @return The top-left position of the physics body in world units.
		 */
		const Math2D::Point2D<float> Physics_GetTopLeftPosition(Entity entity);

		/**
		 * @brief Applies a force to the physics body of the given entity.
		 *
		 * @param entity The entity whose physics body the force will be applied to.
		 * @param force The force vector to apply.
		 */
		void Physics_ApplyForceToBody(Entity entity, Math2D::Point2D<float> force);

		/**
		 * @brief Applies an impulse to the physics body of the given entity.
		 *
		 * @param entity The entity whose physics body the impulse will be applied to.
		 * @param impulse The impulse vector to apply.
		 */
		void Physics_ApplyImpulseToBody(Entity entity, Math2D::Point2D<float> impulse);

		/**
		 * @brief Sets the velocity of the physics body for the given entity.
		 *
		 * @param entity The entity whose physics body velocity is to be set.
		 * @param velocity The velocity vector to apply.
		 */
		void Physics_SetVelocity(Entity entity, const Math2D::Point2D<float> velocity);

		/**
		 * @brief Sets the X-axis velocity of the physics body for the given entity.
		 *
		 * @param entity The entity whose physics body X velocity is to be set.
		 * @param xVelocity The X velocity value.
		 */
		void Physics_SetXVelocity(Entity entity, const float xVelocity);

		/**
		 * @brief Sets the Y-axis velocity of the physics body for the given entity.
		 *
		 * @param entity The entity whose physics body Y velocity is to be set.
		 * @param yVelocity The Y velocity value.
		 */
		void Physics_SetYVelocity(Entity entity, const float yVelocity);

		/**
		 * @brief Sets the deceleration value for the physics body of the given entity.
		 *
		 * @param entity The entity whose physics body deceleration is to be set.
		 * @param decel The deceleration value.
		 */
		void Physics_SetDeceleration(Entity entity, const float decel);

		/**
		 * @brief Gets the velocity of the physics body for the given entity.
		 *
		 * @param entity The entity whose physics body velocity is to be retrieved.
		 * @return The velocity of the physics body.
		 */
		const Math2D::Point2D<float> Physics_GetVelocity(Entity entity);

		/**
		 * @brief Sets the restitution value of the physics body for the given entity.
		 *
		 * @param entity The entity whose physics body restitution is to be set.
		 * @param restitution The restitution coefficient.
		 */
		void Physics_SetRestitution(Entity entity, const float restitution);

		/**
		 * @brief Sets the density of the physics body for the given entity.
		 *
		 * @param entity The entity whose physics body density is to be set.
		 * @param density The density value.
		 */
		void Physics_SetDensity(Entity entity, const float density);

		/**
		 * @brief Sets the friction value of the physics body for the given entity.
		 *
		 * @param entity The entity whose physics body friction is to be set.
		 * @param friction The friction coefficient.
		 */
		void Physics_SetFriction(Entity entity, const float friction);

		/**
		 * @brief Enables or disables fixed rotation for the physics body of the given entity.
		 *
		 * @param entity The entity whose physics body fixed rotation setting is to be modified.
		 * @param fixed True to enable fixed rotation, false to disable it.
		 */
		void Physics_SetFixedRotation(Entity entity, bool fixed);

		/**
		 * @brief Gets the angle of the physics body for the given entity in radians.
		 *
		 * @param entity The entity whose physics body angle is to be retrieved.
		 * @return The angle of the physics body in radians.
		 */
		const float Physics_GetAngleInRadians(Entity entity);

		/**
		 * @brief Gets the angle of the physics body for the given entity in degrees.
		 *
		 * @param entity The entity whose physics body angle is to be retrieved.
		 * @return The angle of the physics body in degrees.
		 */
		const float Physics_GetAngleInDegrees(Entity entity);

		/**
		 * @brief Removes the physics body associated with the given entity from the world.
		 *
		 * @param entity The entity whose physics body is to be removed.
		 */
		void Physics_RemoveBodyFromWorld(Entity entity);

		// Contact System interface

		/**
		* @brief Checks whether the physics body of the given entity has contact below it.
		*
		* @param entity The entity to check.
		* @return True if there is contact below the physics body, false otherwise.
		*/
		const bool Physics_HasContactBelow(Entity entity);

		/**
		 * @brief Checks whether the physics body of the given entity has contact above it.
		 *
		 * @param entity The entity to check.
		 * @return True if there is contact above the physics body, false otherwise.
		 */
		const bool Physics_HasContactAbove(Entity entity);

		/**
		 * @brief Checks whether the physics body of the given entity has contact to the left.
		 *
		 * @param entity The entity to check.
		 * @return True if there is contact to the left of the physics body, false otherwise.
		 */
		const bool Physics_HasContactLeft(Entity entity);

		/**
		 * @brief Checks whether the physics body of the given entity has contact to the right.
		 *
		 * @param entity The entity to check.
		 * @return True if there is contact to the right of the physics body, false otherwise.
		 */
		const bool Physics_HasContactRight(Entity entity);

		////////////////////////////////////////////////////

		//////// Transform Interface functions /////////////

		/**
		* @brief Gets the previous position of the transform component for the given entity.
		*
		* @param entity The entity whose transform component's previous position is to be retrieved.
		* @return The previous position of the transform in world units.
		*/
		const Math2D::Point2D<float> Transform_GetPrevPosition(Entity entity);

		/**
		 * @brief Gets the previous position of the specified transform.
		 *
		 * @param transform The transform whose previous position is to be retrieved.
		 * @return The previous position of the transform in world units.
		 */
		const Math2D::Point2D<float> Transform_GetPrevPosition(Transform& transform);

		/**
		 * @brief Sets the position of the transform component for the given entity.
		 *
		 * @note This function has no effect if the entity has an associated physics body.
		 *
		 * @param entity The entity whose transform position is to be set.
		 * @param position The position to assign to the transform in world units.
		 */
		void Transform_SetPosition(Entity entity, const Math2D::Point2D<float> position);

		/**
		 * @brief Gets the current position of the transform component for the given entity.
		 *
		 * @param entity The entity whose transform position is to be retrieved.
		 * @return The position of the transform in world units.
		 */
		const Math2D::Point2D<float> Transform_GetPosition(Entity entity);

		/**
		 * @brief Sets the Z index of the transform component for the given entity.
		 *
		 * The Z index controls render order, where higher values are rendered in front
		 * of lower values.
		 *
		 * @param entity The entity whose transform Z index is to be set.
		 * @param zIndex The Z index value to assign.
		 */
		void Transform_SetZIndex(Entity entity, const int zIndex);

		/**
		 * @brief Gets the Z index of the transform component for the given entity.
		 *
		 * @param entity The entity whose transform Z index is to be retrieved.
		 * @return The Z index of the transform.
		 */
		const size_t Transform_GetZIndex(Entity entity);

		/**
		 * @brief Sets the rotation of the transform component for the given entity.
		 *
		 * @param entity The entity whose transform rotation is to be set.
		 * @param rotation The rotation angle in degrees.
		 */
		void Transform_SetRotation(Entity entity, const float rotation);

		/**
		 * @brief Gets the rotation of the transform component for the given entity.
		 *
		 * @param entity The entity whose transform rotation is to be retrieved.
		 * @return The rotation of the transform in degrees.
		 */
		const float Transform_GetRotation(Entity entity);

		/**
		 * @brief Sets the scale of the transform component for the given entity.
		 *
		 * @param entity The entity whose transform scale is to be set.
		 * @param scale The scale value to apply.
		 */
		void Transform_SetScale(Entity entity, const float scale);

		/**
		 * @brief Gets the scale of the transform component for the given entity.
		 *
		 * @param entity The entity whose transform scale is to be retrieved.
		 * @return The scale of the transform.
		 */
		const float Transform_GetScale(Entity entity);

		/**
		 * @brief Sets the direction the transform is facing.
		 *
		 * Commonly used for sprite flipping or directional logic
		 * (e.g., 1 for right, -1 for left).
		 *
		 * @param entity The entity whose transform direction is to be set.
		 * @param direction The direction value.
		 */
		void Transform_SetDirectionFacing(Entity entity, const int direction);

		/**
		 * @brief Gets the direction the transform is facing.
		 *
		 * @param entity The entity whose transform direction is to be retrieved.
		 * @return The direction the transform is facing (e.g., 1 or -1).
		 */
		const int Transform_GetDirectionFacing(Entity entity);

		////////////////////////////////////////////////////

		///////////// Screen Interface functions ///////////

		/**
		* @brief Gets the display resolution of the screen.
		*
		* @return The display resolution in pixels.
		*/
		const Math2D::Point2D<int> GetScreenSize();

		/**
		 * @brief Gets the virtual resolution of the screen.
		 *
		 * The virtual screen size is derived from the render output aspect ratio.
		 * The virtual height is fixed at 720 pixels to maintain a consistent and
		 * reasonable aspect ratio across different display resolutions.
		 *
		 * @return The virtual screen resolution.
		 */
		const Math2D::Point2D<int> GetVirtualSize();

		/**
		 * @brief Gets the screen scale factors.
		 *
		 * These values represent the scaling applied from virtual resolution
		 * to actual display resolution.
		 *
		 * @return The screen scale factors.
		 */
		const Math2D::Point2D<float> GetScale();

		/**
		 * @brief Gets the uniform scale constant applied to rendering.
		 *
		 * @return The scale constant.
		 */
		const float GetScaleConstant();

		/**
		 * @brief Gets the current viewport size.
		 *
		 * @return The size of the viewport in pixels.
		 */
		const Math2D::Point2D<int> GetViewportSize();

		/**
		 * @brief Gets the current viewport position.
		 *
		 * @return The position of the viewport in pixels.
		 */
		const Math2D::Point2D<int> GetViewportPosition();

		/**
		 * @brief Sets the window size to the specified dimensions.
		 *
		 * @param size The desired window size in pixels.
		 */
		void SetWindowSize(const Math2D::Point2D<int>& size);

		/**
		 * @brief Sets the window to fullscreen mode.
		 */
		void SetFullscreen();

		////////////////////////////////////////////////////

		////// Templated ECS Interface functions ///////////

		/**
		* @brief Creates a new entity in the ECS.
		* 
		* @return The newly created entity.
		*/
		Entity ECS_CreateEntity();

		/**
		* @brief Adds a component of type @c T to the specified entity.
		*
		* @todo This function is not currently supported.
		* Components must be added via rules files at this time.
		*
		* @tparam T The component type to add.
		* @tparam Args Variadic argument types used to construct the component.
		* @param entity The entity to which the component would be added.
		* @param componentArgs Arguments forwarded to the component constructor.
		*/
		
		template<typename T, typename... Args>
		void AddComponent(Entity entity, Args&&... componentArgs)
		{
			// m_ecs.AddComponent<T>(entity, std::forward<Args>(componentArgs)...);
		}
		

		/**
		* @brief Adds a component of type @c T to all entities matching a tile ID
		* in the specified scene.
		*
		* @note This function is temporarily exposed for scripting components only.
		* Once scripting components can be registered via rules files, this function
		* will be removed from the public API.
		*
		* @tparam T The component type to add.
		* @tparam Args Variadic argument types used to construct the component.
		* @param sceneName The name of the scene to modify.
		* @param tileId The tile ID identifying which entities receive the component.
		* @param componentArgs Arguments forwarded to the component constructor.
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
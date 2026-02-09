#pragma once

#include "ECS/Component.h"
#include "Physics/BodyTypes.h"
#include "AppState.h"
#include "../Public/Includes.h"
#include "../Public/MathUtil.h"

struct b2BodyId;
struct b2ShapeId;
struct b2WorldId;
struct b2ChainId;

namespace Engine
{
	/**
	* @enum DebugColor
	* 
	* @brief Enum representing different debug colors for rendering debug information.
	*/
	enum class DebugColor
	{
		NoColor, /// Default
		Red,	 /// Red debug color
		Green,	 /// Green debug color
		Blue,	 /// Blue debug color
		Black,	 /// Black debug color
	};

	/**
	* @struct Transform
	* 
	* @brief Component representing the universal data that systems need to know about an entity.
	*/
	struct Transform : public Component
	{
		Math2D::Point2D<float> m_prevPosition; /// Previous position of the entity's physics body.
		Math2D::Point2D<float> m_position;	   /// Current position of the entity's physics body.
		Math2D::Point2D<float> m_velocity;	   /// Current velocity of the entity's physics body.

		size_t m_zIndex;			/// Z-index for rendering order.
		float m_rotation;			/// Rotation of the entity in degrees.
		int m_directionFacing;		/// Direction the entity is facing (1 for right, -1 for left).
		bool m_drawDebug;			/// Flag indicating whether to draw debug information for this entity.
		DebugColor m_debugColor;	/// Color to use for debug rendering.

		Transform(Entity entity) : m_prevPosition(0.0f, 0.0f), m_position(0.0f, 0.0f),
			m_rotation(0.0f), m_directionFacing(1), m_zIndex(0), 
			m_drawDebug(false), m_debugColor(DebugColor::Red), Component(entity) {}

		Transform(Entity entity, Math2D::Point2D<float> position, float rotation, 
			int direction, size_t zIndex, const bool drawDebug, DebugColor debugColor) :
			m_prevPosition(position), m_position(position), m_rotation(rotation), 
			m_directionFacing(direction), m_zIndex(zIndex), m_drawDebug(drawDebug),
			m_debugColor(debugColor),
			Component(entity) {}

		~Transform() = default;
	};

	/**
	* @struct PhysicsBody
	* 
	* @brief Component representing the physics body of an entity. A physics body is a box2d body and shape under the hood.
	* These physics bodies are updated by the physics system each tick. First by the box2d world, then the transform position
	* is reflected to match the physics body position.
	*/
	struct PhysicsBody : public Component
	{
		b2BodyId* m_bodyId;								/// Pointer to the Box2D body ID.
		b2ShapeId* m_shapeId;							/// Pointer to the Box2D shape ID.
		b2WorldId* m_worldId;							/// Pointer to the Box2D world ID.

		BodyType m_bodyType;							/// Type of the physics body (STATIC, DYNAMIC, KINEMATIC, SENSOR).
		Filter m_category;								/// Collision category of the physics body.
		Filter m_mask;									/// Collision mask of the physics body.

		bool m_drawDebug;								/// Flag indicating whether to draw debug information for this physics body.
		bool m_fillRect;								/// Flag indicating whether to fill the rectangle when drawing debug information.
		DebugColor m_debugColor;						/// Color to use for debug rendering.

		Math2D::Point2D<float> m_dimensions;			/// Dimensions of the physics body in world units.
		Math2D::Point2D<float> m_halfDimensions;		/// Half dimensions of the physics body in world units.
		Math2D::Point2D<float> m_startingPosition;		/// Starting position of the physics body in world units.
		Math2D::Point2D<float> m_position;				/// Current position of the physics body in world units.

		float m_rotation;								/// Rotation of the physics body in degrees.
		bool m_gravityOn;								/// Flag indicating whether gravity is applied to the physics body.	

		bool m_checkSimpleContacts = false;				/// Flag indicating whether to check for simple contacts (above, below, left, right).

		bool m_contactAbove = false;					/// Flag indicating contact above the physics body. Only valid if m_checkSimpleContacts is true
		bool m_contactBelow = false;					/// Flag indicating contact below the physics body. Only valid if m_checkSimpleContacts is true
		bool m_contactRight = false;					/// Flag indicating contact to the right of the physics body. Only valid if m_checkSimpleContacts is true
		bool m_contactLeft = false;						/// Flag indicating contact to the left of the physics body. Only valid if m_checkSimpleContacts is true

		bool m_enabled;									/// Flag indicating whether the physics body is enabled.

		PhysicsBody(Entity entity) :
			m_bodyId(nullptr), m_enabled(true), m_shapeId(nullptr), m_worldId(nullptr),
			m_bodyType(STATIC), m_dimensions(Math2D::Point2D<float>(1.0f, 1.0f)),
			m_halfDimensions(Math2D::Point2D<float>(0.5f, 0.5f)), m_startingPosition(Math2D::Point2D<float>(1.0f, 1.0f)),
			m_position(Math2D::Point2D<float>(0.0f, 0.0f)), m_rotation(0.0f), m_drawDebug(false), m_fillRect(false), 
			m_debugColor(DebugColor::Red), m_gravityOn(true), m_category(ALL), m_mask(ALL), m_checkSimpleContacts(false), 
			Component(entity) {}

		PhysicsBody(Entity entity, const bool enabled, BodyType bodyType, Filter category, Filter mask,
			Math2D::Point2D<float> dimensions, Math2D::Point2D<float> startingPosition,
			float rotation, bool gravityOn, bool checkSimpleContacts, bool drawDebug, bool fillRect, DebugColor debugColor)
			: m_bodyId(nullptr), m_enabled(enabled), m_shapeId(nullptr), m_worldId(nullptr),
			m_bodyType(bodyType), m_category(category), m_mask(mask), m_dimensions(dimensions),
			m_halfDimensions(dimensions / 2.0f), m_startingPosition(startingPosition), m_drawDebug(drawDebug), 
			m_fillRect(fillRect), m_debugColor(debugColor), m_rotation(rotation), m_gravityOn(gravityOn), 
			m_checkSimpleContacts(checkSimpleContacts), Component(entity) {}

		~PhysicsBody() = default;
	};

	/**
	* @struct PhysicsUpdater
	* 
	* @brief Component that allows for custom physics update logic via a callback function. The physics system calls this function 
	* after updating the physics bodies each tick, allowing for additional physics-related updates or behaviors to be implemented.
	*/
	struct PhysicsUpdater : public Component
	{
		using UpdateCallback = std::function<void(Entity entity)>;  /// Callback function type for updating physics.

		UpdateCallback m_callback;								    /// Callback function for updating physics.

		/**
		* @brief Calls the update callback function for the physics updater.
		* 
		* @param entity The entity associated with the physics updater.
		*/
		void Update(Entity entity)			
		{
			if (m_callback)
			{
				m_callback(entity);
			}
		}

		PhysicsUpdater(Entity entity, UpdateCallback callback) : m_callback(callback), Component(entity) {}
		~PhysicsUpdater() = default;
	};

	/**
	* @struct RenderObject
	* 
	* @brief Struct representing a renderable object in the rendering system. The camera system uses RenderObjects to store
	* information about what to render on the screen. The camera system then stores these objects in their render buckets 
	* for rendering.
	*/
	struct RenderObject
	{
		size_t m_entity;										/// Entity ID associated with the render object.
		Math2D::Point2D<int> m_locationInPixelsOnScreen;		/// Location of the render object on the screen in pixels.
		Math2D::Point2D<int> m_sizeInPixelsOnScreen;			/// Size of the render object on the screen in pixels.
		Math2D::Point2D<int> m_locationInPixelsOnSpriteSheet;	/// Location of the render object on the sprite sheet in pixels.
		Math2D::Point2D<int> m_sizeInPixelsOnSpriteSheet;		/// Size of the render object on the sprite sheet in pixels.

		RenderObject(size_t entity, Math2D::Point2D<int> locationInPixelsOnScreen, Math2D::Point2D<int> sizeInPixelsOnScreen, 
			Math2D::Point2D<int> locationInPixelsOnSpriteSheet, Math2D::Point2D<int> sizeInPixelsOnSpriteSheet)
			: m_entity(entity), m_locationInPixelsOnScreen(locationInPixelsOnScreen), m_sizeInPixelsOnScreen(sizeInPixelsOnScreen),
			m_locationInPixelsOnSpriteSheet(locationInPixelsOnSpriteSheet), m_sizeInPixelsOnSpriteSheet(sizeInPixelsOnSpriteSheet) {}
	};

	/**
	* @struct DebugObject
	* 
	* @brief Struct representing a debug renderable object in the rendering system. The camera system uses DebugObjects to store
	* information about what debug shapes to render on the screen. The camera system then stores these objects in their debug render buckets
	* for rendering. 
	*/
	struct DebugObject
	{
		size_t m_entity;									/// Entity ID associated with the debug object.
		bool m_filled;										/// Flag indicating whether the debug shape should be filled.
		DebugColor m_debugColor;							/// Color to use for debug rendering. 
		Math2D::Point2D<int> m_locationInPixelsOnScreen;	/// Location of the debug object on the screen in pixels.
		Math2D::Point2D<int> m_sizeInPixelsOnScreen;		/// Size of the debug object on the screen in pixels.

		DebugObject(size_t entity, bool filled, Math2D::Point2D<int> locationInPixelsOnScreen, 
			Math2D::Point2D<int> sizeInPixelsOnScreen, DebugColor debugColor)
			: m_entity(entity), m_filled(filled), m_locationInPixelsOnScreen(locationInPixelsOnScreen), 
			m_sizeInPixelsOnScreen(sizeInPixelsOnScreen), m_debugColor(debugColor) {}
	};

	/**
	* @struct LineObject
	* 
	* @brief Struct representing a debug line object in the rendering system. The camera system uses LineObjects to store
	* information about what debug lines to render on the screen. The camera system then stores these objects in their debug lines render buckets
	* for rendering.
	*/
	struct LineObject
	{
		size_t m_entity;									/// Entity ID associated with the line object.
		Math2D::Point2D<int> m_startPointInPixelsOnScreen;	/// Start point of the line on the screen in pixels.
		Math2D::Point2D<int> m_endPointInPixelsOnScreen;	/// End point of the line on the screen in pixels.
		DebugColor m_debugColor;							/// Color to use for debug rendering.

		LineObject(size_t entity, Math2D::Point2D<int> startPointInPixelsOnScreen, 
			Math2D::Point2D<int> endPointInPixelsOnScreen, DebugColor debugColor)
			: m_entity(entity), m_startPointInPixelsOnScreen(startPointInPixelsOnScreen), 
			m_endPointInPixelsOnScreen(endPointInPixelsOnScreen), m_debugColor(debugColor) {}
	};

	/**
	* @struct DebugPointObject
	* 
	* @brief Struct representing a debug point object in the rendering system. The camera system uses DebugPointObjects to store
	* information about what debug points to render on the screen. The camera system then stores these objects in their debug points render buckets
	* for rendering.
	*/
	struct DebugPointObject
	{
		size_t m_entity;									/// Entity ID associated with the debug point object.
		Math2D::Point2D<int> m_locationInPixelsOnScreen;	/// Location of the debug point on the screen in pixels.
		DebugColor m_debugColor;							/// Color to use for debug rendering.

		DebugPointObject(size_t entity, Math2D::Point2D<int> pointInPixelsOnScreen, DebugColor debugColor)
			: m_entity(entity), m_locationInPixelsOnScreen(pointInPixelsOnScreen), m_debugColor(debugColor) {}
	};

	using RenderBucket = std::vector<std::vector<RenderObject>>;				/// Vector index is the zIndex.
	using DebugRenderBucket = std::vector<std::vector<DebugObject>>;			/// Vector index is the zIndex.
	using LinesRenderBucket = std::vector<std::vector<LineObject>>;				/// Vector index is the zIndex.
	using DebugPointRenderBucket = std::vector<std::vector<DebugPointObject>>;  /// Vector index is the zIndex.
	
	/**
	* @struct Camera
	*
	* @brief Component representing a camera in the rendering system. The camera component holds information about the camera's
	* position, size, and rendering buckets for storing renderable objects. The camera system processes this component to 
	* determine what to render on the screen.
	*/
	struct Camera : public Component
	{

		Math2D::Point2D<float> m_viewPortPositionInPercentageOfScreen;				 /// Position of the camera in fraction of screen size (0.0 - 1.0).
		Math2D::Point2D<float> m_viewportSizeInPercentageOfScreen;					 /// Screen ratio of the camera (width / height).

		Math2D::Point2D<float> m_offset;						 /// Top left position of the camera in world units.
		Math2D::Point2D<float> m_size;							 /// Size of the camera in world units.

		size_t m_pixelsPerUnit;									 /// Number of pixels per world unit.
		bool m_clampingOn;										 /// Flag indicating whether clamping is enabled for the camera.
		Math2D::Point2D<int> m_bounds;							 /// Bounds of the camera in pixels.
		bool m_borderOn;										 /// Flag indicating whether to draw a border around the camera view.
		size_t m_numLayers;										 /// Number of layers for rendering.
		std::array<size_t, 3> m_backgroundColor;				 /// Background color of the camera in RGB format.

		RenderBucket m_renderBucket;							 /// Bucket for storing renderable objects. Maps zIndex to vector of RenderObjects.
		DebugRenderBucket m_debugRenderBucket;					 /// Bucket for storing renderable debug objects. Maps zIndex to vector of DebugObjects.
		LinesRenderBucket m_debugLinesRenderBucket;				 /// Bucket for storing renderable debug lines. Maps zIndex to vector of lines.
		DebugPointRenderBucket m_debugPointsRenderBucket;		 /// Bucket for storing renderable point objects. Maps zIndex to vector of debug points.

		Camera(Entity entity)
			: m_offset(0.0f, 0.0f), m_size(0.0f, 0.0f), m_viewportSizeInPercentageOfScreen(1.0f, 1.0f), m_numLayers(10),
			m_viewPortPositionInPercentageOfScreen(0.0f, 0.0f), m_pixelsPerUnit(32), m_clampingOn(true), m_borderOn(false),
			m_bounds(0, 0), m_renderBucket(10, std::vector<RenderObject>()), m_backgroundColor({ 0, 0, 0 }),
			m_debugRenderBucket(10, std::vector<DebugObject>()), m_debugLinesRenderBucket(10, std::vector<LineObject>()),
			m_debugPointsRenderBucket(10, std::vector<DebugPointObject>()),
			Component(entity) {}

		Camera(Entity entity, Math2D::Point2D<float> size, Math2D::Point2D<float> screenRatio, 
			Math2D::Point2D<float> position, size_t pixelsPerUnit, bool clampingOn, bool border, std::array<size_t, 3> backgroundColor, size_t numLayers)
			: m_size(size), m_viewportSizeInPercentageOfScreen(screenRatio), m_viewPortPositionInPercentageOfScreen(position),
			m_pixelsPerUnit(pixelsPerUnit), m_clampingOn(clampingOn), m_offset(0.0f, 0.0f), m_bounds(0, 0), 
			m_borderOn(border), m_renderBucket(numLayers, std::vector<RenderObject>()), m_backgroundColor(backgroundColor),
			m_numLayers(numLayers), m_debugRenderBucket(numLayers, std::vector<DebugObject>()), 
			m_debugLinesRenderBucket(numLayers, std::vector<LineObject>()), m_debugPointsRenderBucket(numLayers, std::vector<DebugPointObject>()),
			Component(entity) {}
	};

	/**
	* @struct CameraUpdater
	* 
	* @brief Component that allows for custom camera update logic via a callback function. The camera system calls this function
	* each tick, allowing for dynamic camera behaviors such as following an entity or implementing camera effects.
	*/
	struct CameraUpdater : public Component
	{
		using UpdateCallback = std::function<void(Entity entity)>;  /// Callback function type for updating camera.

		UpdateCallback m_callback; 	/// Callback function for updating camera.

		/**
		* @brief Calls the update callback function for the camera updater.
		*
		* @param entity The entity associated with the camera updater.
		*/
		void Update(Entity entity)
		{
			if (m_callback)
			{
				m_callback(entity);
			}
		}

		CameraUpdater(Entity entity, UpdateCallback callback) : m_callback(callback), Component(entity) {}
		~CameraUpdater() = default;
	};

	/**
	* @struct ChainCollider
	* 
	* @brief Component representing a chain collider in the physics system. A chain collider is a series of connected line segments
	* that can be used to create complex collision shapes. This component holds the necessary data for creating and managing
	* a chain collider in the physics system.
	*/
	struct ChainCollider : public Component
	{
		Math2D::Chain m_chain;				/// Points and edges defining the chain collider.

		b2BodyId* m_bodyId = nullptr;		/// Pointer to the Box2D body ID.
		b2ChainId* m_chainId = nullptr;		/// Pointer to the Box2D chain ID.
		b2WorldId* m_worldId = nullptr;		/// Pointer to the Box2D world ID.

		Filter m_category;			/// Collision category of the chain collider.
		Filter m_mask;				/// Collision mask of the chain collider.

		bool m_loop = false;		/// Whether the chain collider is a loop.

		bool m_drawDebug;			/// Flag indicating whether to draw debug information for this chain collider.
		DebugColor m_debugColor;	/// Color to use for debug rendering.

		bool m_enabled;				/// Flag indicating whether the chain collider is enabled.

		ChainCollider(Entity entity, Math2D::Chain refPoints, 
			const bool enabled, Filter category, Filter mask, bool drawDebug, DebugColor debugColor)
			: m_chain(refPoints), m_category(category), m_enabled(enabled), m_mask(mask), 
			m_drawDebug(true), m_debugColor(debugColor), Component(entity) {}
		~ChainCollider() = default;
	};

	/**
	* @struct Sprite
	*
	* @brief Component representing a sprite in the rendering system. A sprite is a 2D image or animation that can be rendered on the screen.
	*/
	struct Sprite : public Component
	{
		void* m_ptrLoadedTexture;								/// Pointer to the loaded texture for the sprite.
		Math2D::Point2D<int> m_pixelsPerFrame;					/// Size of each frame in the sprite sheet in pixels.
		Math2D::Point2D<int> m_locationInPixelsOnSpriteSheet;	/// Location of the sprite frame on the sprite sheet in pixels.
		Math2D::Point2D<float> m_sizeInUnits;					/// Size of the sprite in world units.
		Math2D::Point2D<float> m_offsetFromTransform;			/// Offset of the sprite from the entity's transform in world units.
		Math2D::Point2D<size_t> m_dimensions;					/// Dimensions of the sprite sheet in frames (width, height)

		bool m_drawDebug;										/// Flag indicating whether to draw debug information for this sprite.
		DebugColor m_debugColor;								/// Color to use for debug rendering.

		Sprite(Entity entity, void* ptrLoadedTexture, Math2D::Point2D<int> pixelsPerFrame,
			Math2D::Point2D<float> offsetFromTransform, Math2D::Point2D<size_t> dimensions,
			Math2D::Point2D<float> size, const bool drawDebug, DebugColor debugColor)
			: m_ptrLoadedTexture(ptrLoadedTexture), m_offsetFromTransform(offsetFromTransform),
			m_pixelsPerFrame(pixelsPerFrame), m_dimensions(dimensions), m_sizeInUnits(size),
			m_drawDebug(drawDebug), m_debugColor(debugColor), m_locationInPixelsOnSpriteSheet(0, 0),
			Component(entity) {}

		~Sprite() = default;
	};

	/**
	* @struct Animation
	* 
	* @brief Struct representing an animation for a sprite. An animation is a sequence of frames that can be played to create
	* motion or visual effects.
	*/
	struct Animation
	{
		size_t m_id;					/// Name of animation. 
		std::vector<int> m_frames;		/// Frames in the animation. Each int corresponds to a frame index in the sprite sheet.
		size_t m_numFrames;				/// Number of frames in the animation.
		size_t m_frameTime = 0;			/// Time since the last frame change.
		size_t m_frameDuration;			/// Duration each frame is displayed (in ticks).
		int m_currentFrame = 0;         /// Current frame in the current animation
		size_t m_frameCounter = 0;      /// Tracks the number of frames passed since the last frame change 
		bool m_loop;					/// Whether the animation should loop.

		Animation() = default;
		Animation(size_t id, std::vector<int> frames, int frameDuration,  bool loop)
			: m_id(id), m_frames(frames), m_numFrames(frames.size()), m_frameTime(0),
			m_frameDuration(frameDuration), m_loop(loop) {};
	
	};

	/**
	* @struct Animations
	* 
	* @brief Component representing a collection of animations for a sprite. This component holds all the animations
	* available for a sprite and tracks the current animation being played.
	*/
	struct Animations : public Component
	{
		std::unordered_map<size_t, Animation> m_animations; /// All animations for this sprite
		size_t m_currentAnimation;							 /// Name of the current animation being played

		Animations(Entity entity, std::unordered_map<size_t, Animation> animations)
			:
			m_animations(animations), 
			m_currentAnimation(0),
			Component(entity) {}

		~Animations() = default;
	};

	/**
	* @struct AudioSource
	* 
	* @brief Component representing an audio source in the audio system. An audio source is responsible for playing
	* sound effects or music in the game associated with an entity.
	*/
	struct AudioSource : public Component
	{
		bool m_enabled;				/// Whether the audio source is enabled.
		bool m_loop;				/// Whether the audio should loop when played.
		int m_volume;				/// 0 - 128
		std::string m_soundName;	/// Name of the sound to be played by the audio source.

		AudioSource(Entity entity, const std::string& soundName)
			: m_enabled(false), m_loop(false), m_volume(0), m_soundName(soundName), Component(entity) {}

		~AudioSource() = default;
	};
}
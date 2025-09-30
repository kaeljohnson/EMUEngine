#pragma once

#include "ECS/Component.h"
#include "Physics/BodyTypes.h"
#include "AppState.h"
#include "Includes.h"
#include "MathUtil.h"
#include "Core.h"
#include "Logging/Logger.h"

struct b2BodyId;
struct b2ShapeId;
struct b2WorldId;
struct b2ChainId;

namespace Engine
{
	struct Transform : public Component
	{
		Vector2D<float> PrevPosition;	// Previous position of transform in world space.
		Vector2D<float> Position;		// Current position of transform in world space.
		Vector2D<float> Velocity;
		Vector2D<float> Dimensions;

		int ZIndex;
		float Rotation;
		float Scale;
		int DirectionFacing;
		bool m_drawDebug;

		Transform(Entity entity) : PrevPosition(0.0f, 0.0f), Position(0.0f, 0.0f),
			Dimensions(0.0f, 0.0f), Rotation(0.0f), Scale(1.0f), DirectionFacing(1), ZIndex(0), m_drawDebug(false), Component(entity) {}

		Transform(Entity entity, Vector2D<float> position, Vector2D<float> dimensions, float rotation, float scale, int direction, int zIndex, const bool drawDebug) :
			PrevPosition(position), Position(position),
			Dimensions(dimensions), Rotation(rotation), Scale(scale), 
			DirectionFacing(direction), ZIndex(zIndex), m_drawDebug(drawDebug),
			Component(entity) {}

		~Transform() = default;
	};

	struct PhysicsBody : public Component
	{
		PhysicsBody(Entity entity) :
			m_bodyId(nullptr), m_enabled(true), m_shapeId(nullptr), m_worldId(nullptr),
			m_bodyType(STATIC), m_dimensions(Vector2D<float>(1.0f, 1.0f)),
			m_halfDimensions(Vector2D<float>(0.5f, 0.5f)), m_startingPosition(Vector2D<float>(1.0f, 1.0f)),
			m_position(Vector2D<float>(0.0f, 0.0f)), m_rotation(0.0f),
			m_gravityOn(true), m_category(ALL), m_mask(ALL), m_checkSimpleContacts(false), Component(entity) {}

		PhysicsBody(Entity entity, const bool enabled, BodyType bodyType, Filter category, Filter mask,
			Vector2D<float> dimensions, Vector2D<float> startingPosition,
			float rotation, bool gravityOn, bool checkSimpleContacts)
			: m_bodyId(nullptr), m_enabled(enabled), m_shapeId(nullptr), m_worldId(nullptr),
			m_bodyType(bodyType), m_category(category), m_mask(mask), m_dimensions(dimensions),
			m_halfDimensions(dimensions / 2.0f), m_startingPosition(startingPosition),
			m_rotation(rotation), m_gravityOn(gravityOn), m_checkSimpleContacts(checkSimpleContacts), Component(entity) {}

		~PhysicsBody() = default;

		b2BodyId* m_bodyId;
		b2ShapeId* m_shapeId;
		b2WorldId* m_worldId;

		BodyType m_bodyType;
		Filter m_category;
		Filter m_mask;

		Vector2D<float> m_dimensions;
		Vector2D<float> m_halfDimensions;
		Vector2D<float> m_startingPosition;
		Vector2D<float> m_position;

		float m_rotation;
		bool m_gravityOn;

		bool m_checkSimpleContacts = false; // If true, the physics system will check for simple contacts and set the contact booleans below to true or false.	

		bool m_contactAbove = false;
		bool m_contactBelow = false;
		bool m_contactRight = false;
		bool m_contactLeft = false;

		bool m_enabled;
	};

	struct PhysicsUpdater : public Component
	{
		using UpdateCallback = std::function<void(Entity entity)>;

		PhysicsUpdater(Entity entity, UpdateCallback callback) : m_callback(callback), Component(entity) {}
		~PhysicsUpdater() = default;

		UpdateCallback m_callback;

		void Update(Entity entity)
		{
			if (m_callback)
			{
				m_callback(entity);
			}
		}
	};

	struct RenderObject
	{
		RenderObject(size_t entity, Vector2D<int> locationInPixelsOnScreen, Vector2D<int> sizeInPixelsOnScreen, Vector2D<int> locationInPixelsOnSpriteSheet, Vector2D<int> sizeInPixelsOnSpriteSheet)
			: m_entity(entity), m_locationInPixelsOnScreen(locationInPixelsOnScreen), m_sizeInPixelsOnScreen(sizeInPixelsOnScreen),
			m_locationInPixelsOnSpriteSheet(locationInPixelsOnSpriteSheet), m_sizeInPixelsOnSpriteSheet(sizeInPixelsOnSpriteSheet) {}

		size_t m_entity;
		Vector2D<int> m_locationInPixelsOnScreen;
		Vector2D<int> m_sizeInPixelsOnScreen;
		Vector2D<int> m_locationInPixelsOnSpriteSheet;
		Vector2D<int> m_sizeInPixelsOnSpriteSheet;
	};

	struct DebugObject
	{
		DebugObject(size_t entity, bool filled, Vector2D<int> locationInPixelsOnScreen, Vector2D<int> sizeInPixelsOnScreen)
			: m_entity(entity), m_filled(filled), m_locationInPixelsOnScreen(locationInPixelsOnScreen), m_sizeInPixelsOnScreen(sizeInPixelsOnScreen) {}

		size_t m_entity;
		bool m_filled;
		Vector2D<int> m_locationInPixelsOnScreen;
		Vector2D<int> m_sizeInPixelsOnScreen;
	};

	struct LineObject
	{
		LineObject(size_t entity, Vector2D<int> startPointInPixelsOnScreen, Vector2D<int> endPointInPixelsOnScreen)
			: m_entity(entity), m_startPointInPixelsOnScreen(startPointInPixelsOnScreen), m_endPointInPixelsOnScreen(endPointInPixelsOnScreen) {}

		size_t m_entity;
		Vector2D<int> m_startPointInPixelsOnScreen;
		Vector2D<int> m_endPointInPixelsOnScreen;
	};

	using RenderBucket = std::map<size_t, std::vector<RenderObject>>;
	using DebugRenderBucket = std::map<size_t, std::vector<DebugObject>>;
	using LinesRenderBucket = std::map<size_t, std::vector<LineObject>>;

	struct Camera : public Component
	{
		Camera(Entity entity)
			: m_offset(0.0f, 0.0f), m_size(0.0f, 0.0f), m_screenRatio(1.0f, 1.0f), 
			m_positionInFractionOfScreenSize(0.0f, 0.0f), m_pixelsPerUnit(32), m_clampingOn(true), 
			m_bounds(0, 0), m_clipRectPosition(0, 0), m_clipRectSize(0, 0),
			Component(entity) {}
		Camera(Entity entity, Vector2D<float> size, Vector2D<float> screenRatio, Vector2D<float> position, int pixelsPerUnit, bool clampingOn)
			: m_size(size), m_screenRatio(screenRatio), m_positionInFractionOfScreenSize(position), 
			m_pixelsPerUnit(pixelsPerUnit), m_clampingOn(clampingOn), m_offset(0.0f, 0.0f), m_bounds(0, 0),
			m_clipRectPosition(0, 0), m_clipRectSize(0, 0), 
			Component(entity) {}
		

		// Window:
		//     Position: position of the camera on the screen.
		//     ScreenRatio: size of the camera in percent of screen.
		Vector2D<float> m_positionInFractionOfScreenSize;
		Vector2D<float> m_screenRatio;

		Vector2D<float> m_offset;		// Top left position of the camera in world space.
		Vector2D<float> m_size;			// Size of the camera in world units.
		
		int m_pixelsPerUnit;
		bool m_clampingOn;
		Vector2D<int> m_bounds;

		Vector2D<int> m_clipRectPosition;
		Vector2D<int> m_clipRectSize;

		RenderBucket m_renderBucket;				// Map of zIndex to vector of RenderObjects.
		DebugRenderBucket m_debugRenderBucket;		// Map of zIndex to vector of DebugObjects.
		LinesRenderBucket m_debugLinesRenderBucket;	// Map of zIndex to vector of lines (2 points per line).
	};

	struct CameraUpdater : public Component
	{
		using UpdateCallback = std::function<void(Entity entity)>;

		CameraUpdater(Entity entity, UpdateCallback callback) : m_callback(callback), Component(entity) {}
		~CameraUpdater() = default;

		UpdateCallback m_callback;

		void Update(Entity entity)
		{
			if (m_callback)
			{
				m_callback(entity);
			}
		}
	};

	struct ChainCollider : public Component
	{
		ChainCollider(Entity entity, const bool enabled, Filter category, Filter mask, Vector2D<float> firstPoint, Vector2D<float> secondPoint,
																   Vector2D<float> thirdPoint, Vector2D<float> fourthPoint)
			: m_category(category), m_enabled(enabled), m_mask(mask), Component(entity) 
		{
			m_points[0] = firstPoint;
			m_points[1] = secondPoint;
			m_points[2] = thirdPoint;
			m_points[3] = fourthPoint;

		}
		~ChainCollider() = default;

		Vector2D<float> m_points[4];

		b2BodyId* m_bodyId = nullptr;
		b2ChainId* m_chainId = nullptr;
		b2WorldId* m_worldId = nullptr;

		Filter m_category;
		Filter m_mask;

		bool m_enabled;

	};

	struct ChainColliderLeft : public ChainCollider 
	{
		ChainColliderLeft(Entity entity, const bool enabled, Filter category, Filter mask, Vector2D<float> firstPoint, Vector2D<float> secondPoint,
			Vector2D<float> thirdPoint, Vector2D<float> fourthPoint) : ChainCollider(entity, enabled, category, mask, firstPoint, secondPoint, thirdPoint, fourthPoint) {}
		~ChainColliderLeft() = default;
	};
	struct ChainColliderRight : public ChainCollider 
	{
		ChainColliderRight(Entity entity, const bool enabled, Filter category, Filter mask, Vector2D<float> firstPoint, Vector2D<float> secondPoint,
			Vector2D<float> thirdPoint, Vector2D<float> fourthPoint) : ChainCollider(entity, enabled, category, mask, firstPoint, secondPoint, thirdPoint, fourthPoint) {}
		~ChainColliderRight() = default;
	};
	struct ChainColliderTop : public ChainCollider 
	{
		ChainColliderTop(Entity entity, const bool enabled, Filter category, Filter mask, Vector2D<float> firstPoint, Vector2D<float> secondPoint,
			Vector2D<float> thirdPoint, Vector2D<float> fourthPoint) : ChainCollider(entity, enabled, category, mask, firstPoint, secondPoint, thirdPoint, fourthPoint) {}
		~ChainColliderTop() = default;
	};
	struct ChainColliderBottom : public ChainCollider 
	{
		ChainColliderBottom(Entity entity, const bool enabled, Filter category, Filter mask, Vector2D<float> firstPoint, Vector2D<float> secondPoint,
			Vector2D<float> thirdPoint, Vector2D<float> fourthPoint) : ChainCollider(entity, enabled, category, mask, firstPoint, secondPoint, thirdPoint, fourthPoint) {}
		~ChainColliderBottom() = default;
	};

	struct Animation
	{
		Animation() = default;
		Animation(std::string name, std::vector<int> frames, int frameDuration, 
			Vector2D<int> pixelsPerFrame, Vector2D<float> offsetFromTransform, 
			Vector2D<size_t> dimensions, bool loop, Vector2D<float> size)
			: m_name(name), m_frames(frames), m_numFrames(frames.size()), m_frameTime(0), 
			m_frameDuration(frameDuration), m_pixelsPerFrame(pixelsPerFrame), 
			m_dimensions(dimensions), m_loop(loop), m_size(size)
		{
		};
		
		std::string m_name;
		std::vector<int> m_frames;
		size_t m_numFrames;
		size_t m_frameTime = 0;
		size_t m_frameDuration;
		int m_currentFrame = 0;          // current frame in the current animation
		size_t m_frameCounter = 0;       // tracks the number of frames passed since the last frame change
		Vector2D<int> m_pixelsPerFrame;
		Vector2D<size_t> m_dimensions;	 // Dimensions of the sprite sheet in frames (width, height)
		Vector2D<float> m_size;
		
		Vector2D<float> m_offsetFromTransform;
		bool m_loop;
	};

	struct Animations : public Component
	{
		Animations(Entity entity, std::unordered_map<std::string, Animation> animations)
			:
			m_animations(animations), 
			m_currentAnimation("Idle"), // Idle for now, need a animation interface for client to set animation in a state machine.
			Component(entity) 
		{}

		std::unordered_map<std::string, Animation> m_animations; // All animations for this sprite
		std::string m_currentAnimation;  // Name of the current animation being played
		
	};
}
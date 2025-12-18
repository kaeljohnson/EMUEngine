#pragma once

#include "ECS/Component.h"
#include "Physics/BodyTypes.h"
#include "AppState.h"
#include "Includes.h"
#include "MathUtil.h"
#include "Logging/Logger.h"

struct b2BodyId;
struct b2ShapeId;
struct b2WorldId;
struct b2ChainId;

namespace Engine
{
	enum class DebugColor
	{
		NoColor,
		Red,
		Green,
		Blue,
		Black
	};

	struct Transform : public Component
	{
		Math2D::Point2D<float> m_prevPosition;
		Math2D::Point2D<float> m_position;
		Math2D::Point2D<float> m_velocity;
		Math2D::Point2D<float> m_dimensions;

		size_t m_zIndex;
		float m_rotation;
		float m_scale;
		int m_directionFacing;
		bool m_drawDebug;
		DebugColor m_debugColor;

		Transform(Entity entity) : m_prevPosition(0.0f, 0.0f), m_position(0.0f, 0.0f),
			m_dimensions(0.0f, 0.0f), m_rotation(0.0f), m_scale(1.0f), m_directionFacing(1), m_zIndex(0), 
			m_drawDebug(false), m_debugColor(DebugColor::Red), Component(entity) {}

		Transform(Entity entity, Math2D::Point2D<float> position, Math2D::Point2D<float> dimensions, float rotation, 
			float scale, int direction, size_t zIndex, const bool drawDebug, DebugColor debugColor) :
			m_prevPosition(position), m_position(position),
			m_dimensions(dimensions), m_rotation(rotation), m_scale(scale), 
			m_directionFacing(direction), m_zIndex(zIndex), m_drawDebug(drawDebug),
			m_debugColor(debugColor),
			Component(entity) {}

		~Transform() = default;
	};

	struct PhysicsBody : public Component
	{
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

		b2BodyId* m_bodyId;
		b2ShapeId* m_shapeId;
		b2WorldId* m_worldId;

		BodyType m_bodyType;
		Filter m_category;
		Filter m_mask;

		bool m_drawDebug; 
		bool m_fillRect;
		DebugColor m_debugColor;

		Math2D::Point2D<float> m_dimensions;
		Math2D::Point2D<float> m_halfDimensions;
		Math2D::Point2D<float> m_startingPosition;
		Math2D::Point2D<float> m_position;

		float m_rotation;
		bool m_gravityOn;

		bool m_checkSimpleContacts = false;

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
		RenderObject(size_t entity, Math2D::Point2D<int> locationInPixelsOnScreen, Math2D::Point2D<int> sizeInPixelsOnScreen, 
			Math2D::Point2D<int> locationInPixelsOnSpriteSheet, Math2D::Point2D<int> sizeInPixelsOnSpriteSheet)
			: m_entity(entity), m_locationInPixelsOnScreen(locationInPixelsOnScreen), m_sizeInPixelsOnScreen(sizeInPixelsOnScreen),
			m_locationInPixelsOnSpriteSheet(locationInPixelsOnSpriteSheet), m_sizeInPixelsOnSpriteSheet(sizeInPixelsOnSpriteSheet) {}

		size_t m_entity;
		Math2D::Point2D<int> m_locationInPixelsOnScreen;
		Math2D::Point2D<int> m_sizeInPixelsOnScreen;
		Math2D::Point2D<int> m_locationInPixelsOnSpriteSheet;
		Math2D::Point2D<int> m_sizeInPixelsOnSpriteSheet;
	};

	struct DebugObject
	{
		DebugObject(size_t entity, bool filled, Math2D::Point2D<int> locationInPixelsOnScreen, 
			Math2D::Point2D<int> sizeInPixelsOnScreen, DebugColor debugColor)
			: m_entity(entity), m_filled(filled), m_locationInPixelsOnScreen(locationInPixelsOnScreen), 
			m_sizeInPixelsOnScreen(sizeInPixelsOnScreen), m_debugColor(debugColor) {}

		size_t m_entity;
		bool m_filled;
		DebugColor m_debugColor;
		Math2D::Point2D<int> m_locationInPixelsOnScreen;
		Math2D::Point2D<int> m_sizeInPixelsOnScreen;
	};

	struct LineObject
	{
		LineObject(size_t entity, Math2D::Point2D<int> startPointInPixelsOnScreen, 
			Math2D::Point2D<int> endPointInPixelsOnScreen, DebugColor debugColor)
			: m_entity(entity), m_startPointInPixelsOnScreen(startPointInPixelsOnScreen), 
			m_endPointInPixelsOnScreen(endPointInPixelsOnScreen), m_debugColor(debugColor) {}

		size_t m_entity;
		Math2D::Point2D<int> m_startPointInPixelsOnScreen;
		Math2D::Point2D<int> m_endPointInPixelsOnScreen;
		DebugColor m_debugColor;
	};

	using RenderBucket = std::vector<std::vector<RenderObject>>;      // Vector index is the zIndex.
	using DebugRenderBucket = std::vector<std::vector<DebugObject>>;  // Vector index is the zIndex.
	using LinesRenderBucket = std::vector<std::vector<LineObject>>;	  // Vector index is the zIndex.

	struct Camera : public Component
	{
		Camera(Entity entity)
			: m_offset(0.0f, 0.0f), m_size(0.0f, 0.0f), m_screenRatio(1.0f, 1.0f), m_numLayers(10),
			m_positionInFractionOfScreenSize(0.0f, 0.0f), m_pixelsPerUnit(32), m_clampingOn(true), m_borderOn(false),
			m_bounds(0, 0), m_clipRectPosition(0, 0), m_clipRectSize(0, 0), m_renderBucket(10, std::vector<RenderObject>()),
			m_debugRenderBucket(10, std::vector<DebugObject>()), m_debugLinesRenderBucket(10, std::vector<LineObject>()),
			Component(entity) 
		{
		}
		Camera(Entity entity, Math2D::Point2D<float> size, Math2D::Point2D<float> screenRatio, 
			Math2D::Point2D<float> position, size_t pixelsPerUnit, bool clampingOn, size_t numLayers)
			: m_size(size), m_screenRatio(screenRatio), m_positionInFractionOfScreenSize(position), 
			m_pixelsPerUnit(pixelsPerUnit), m_clampingOn(clampingOn), m_offset(0.0f, 0.0f), m_bounds(0, 0), 
			m_borderOn(false), m_clipRectPosition(0, 0), m_clipRectSize(0, 0), m_renderBucket(numLayers, std::vector<RenderObject>()), 
			m_numLayers(numLayers), m_debugRenderBucket(numLayers, std::vector<DebugObject>()), 
			m_debugLinesRenderBucket(numLayers, std::vector<LineObject>()),
			Component(entity) {}
		

		// Window:
		//     Position: position of the camera on the screen.
		//     ScreenRatio: size of the camera in percent of screen.
		Math2D::Point2D<float> m_positionInFractionOfScreenSize;
		Math2D::Point2D<float> m_screenRatio;

		Math2D::Point2D<float> m_offset;		// Top left position of the camera in world units.
		Math2D::Point2D<float> m_size;			// Size of the camera in world units.
		
		size_t m_pixelsPerUnit;
		bool m_clampingOn;
		Math2D::Point2D<int> m_bounds;
		bool m_borderOn;
		size_t m_numLayers;

		Math2D::Point2D<int> m_clipRectPosition;
		Math2D::Point2D<int> m_clipRectSize;

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
		ChainCollider(Entity entity, Math2D::Chain refPoints, 
			const bool enabled, Filter category, Filter mask, bool drawDebug, DebugColor debugColor)
			: m_chain(refPoints), m_category(category), m_enabled(enabled), m_mask(mask), 
			m_drawDebug(true), m_debugColor(debugColor), Component(entity) {}
		~ChainCollider() = default;

		Math2D::Chain m_chain;

		b2BodyId* m_bodyId = nullptr;
		b2ChainId* m_chainId = nullptr;
		b2WorldId* m_worldId = nullptr;

		Filter m_category;
		Filter m_mask;

		bool m_loop = false;

		bool m_drawDebug;
		DebugColor m_debugColor;

		bool m_enabled;
	};

	struct Sprite : public Component
	{
		Sprite(Entity entity, void* ptrLoadedTexture, Math2D::Point2D<int> pixelsPerFrame, 
			Math2D::Point2D<float> offsetFromTransform, Math2D::Point2D<size_t> dimensions, 
			Math2D::Point2D<float> size, const bool drawDebug, DebugColor debugColor)
			: m_ptrLoadedTexture(ptrLoadedTexture), m_offsetFromTransform(offsetFromTransform), 
			m_pixelsPerFrame(pixelsPerFrame), m_dimensions(dimensions), m_sizeInUnits(size), 
			m_drawDebug(drawDebug), m_debugColor(debugColor), Component(entity) {}

		~Sprite() = default;

		void* m_ptrLoadedTexture;
		Math2D::Point2D<int> m_pixelsPerFrame;
		Math2D::Point2D<float> m_sizeInUnits;
		Math2D::Point2D<float> m_offsetFromTransform;
		Math2D::Point2D<size_t> m_dimensions;	 // Dimensions of the sprite sheet in frames (width, height)

		bool m_drawDebug;
		DebugColor m_debugColor;
	};

	struct Animation
	{
		Animation() = default;
		Animation(std::string name, std::vector<int> frames, int frameDuration,  bool loop)
			: m_name(name), m_frames(frames), m_numFrames(frames.size()), m_frameTime(0), 
			m_frameDuration(frameDuration), m_loop(loop) {};
		
		std::string m_name;
		std::vector<int> m_frames;
		size_t m_numFrames;
		size_t m_frameTime = 0;
		size_t m_frameDuration;
		int m_currentFrame = 0;          // current frame in the current animation
		size_t m_frameCounter = 0;       // tracks the number of frames passed since the last frame change 
		bool m_loop;
	};

	struct Animations : public Component
	{
		Animations(Entity entity, std::unordered_map<std::string, Animation> animations)
			:
			m_animations(animations), 
			m_currentAnimation("Idle"), // Idle for now, need a animation interface for client to set animation in a state machine.
			Component(entity) {}

		std::unordered_map<std::string, Animation> m_animations; // All animations for this sprite
		std::string m_currentAnimation;							 // Name of the current animation being played
	};

	struct AudioSource : public Component
	{
		AudioSource(Entity entity, const std::string& soundName)
			: m_enabled(false), m_loop(false), m_volume(0), m_soundName(soundName), Component(entity) {}

		~AudioSource() = default;

		bool m_enabled;
		bool m_loop;
		int m_volume; // 0 - 128
		std::string m_soundName;
	};
}
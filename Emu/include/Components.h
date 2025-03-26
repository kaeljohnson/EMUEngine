#pragma once

#include "ECS/Component.h"
#include "Physics/BodyTypes.h"
#include "AppState.h"
#include "Includes.h"
#include "MathUtil.h"
#include "Core.h"

struct b2BodyId;
struct b2ShapeId;
struct b2WorldId;

namespace Engine
{
	struct Transform : public Component
	{
		Vector2D<float> PrevPosition;
		Vector2D<float> Position;
		Vector2D<float> Velocity;
		Vector2D<float> Dimensions;

		int ZIndex;
		float Rotation;
		float Scale;
		int DirectionFacing;

		EMU_API Transform(Entity entity) : PrevPosition(0.0f, 0.0f), Position(0.0f, 0.0f),
			Dimensions(0.0f, 0.0f), Rotation(0.0f), Scale(1.0f), DirectionFacing(1), ZIndex(0), Component(entity) {}

		EMU_API Transform(Entity entity, Vector2D<float> position, Vector2D<float> dimensions, float rotation, float scale, int direction, int zIndex) :
			PrevPosition(position), Position(position),
			Dimensions(dimensions), Rotation(rotation), Scale(scale), DirectionFacing(direction), ZIndex(zIndex), Component(entity) {}

		EMU_API ~Transform() = default;
	};

	struct PhysicsBody : public Component
	{
		EMU_API PhysicsBody(Entity entity) :
			m_bodyId(nullptr), m_shapeId(nullptr), m_worldId(nullptr),
			m_bodyType(STATIC), m_dimensions(Vector2D<float>(1.0f, 1.0f)),
			m_halfDimensions(Vector2D<float>(0.5f, 0.5f)), m_startingPosition(Vector2D<float>(1.0f, 1.0f)),
			m_position(Vector2D<float>(0.0f, 0.0f)), m_rotation(0.0f),
			m_gravityOn(true), Component(entity) {}

		EMU_API ~PhysicsBody() = default;

		b2BodyId* m_bodyId;
		b2ShapeId* m_shapeId;
		b2WorldId* m_worldId;

		BodyType m_bodyType;

		Vector2D<float> m_dimensions;
		Vector2D<float> m_halfDimensions;
		Vector2D<float> m_startingPosition;
		Vector2D<float> m_position;

		float m_rotation;
		bool m_gravityOn;
	};

    struct Updatable : public Component
    {
        using UpdateCallback = std::function<void()>;

		EMU_API Updatable(Entity entity, UpdateCallback callback) : m_callback(callback), Component(entity) {}
		EMU_API ~Updatable() = default;

        UpdateCallback m_callback;

        void Update()
		{
			if (m_callback)
			{
				m_callback();
			}
		}
    };

	enum ContactDirection
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	struct SimpleContact : public Component
	{
		SimpleContact(Entity entity) : Component(entity) {}

		bool m_contactAbove = false;
		bool m_contactBelow = false;
		bool m_contactRight = false;
		bool m_contactLeft = false;
	};
	
	struct Camera : public Component
	{
		EMU_API Camera(Entity entity) 
			: m_offset(0.0f, 0.0f), m_size(0.0f, 0.0f), m_pixelsPerUnit(32), m_clampingOn(true), Component(entity) {}
		EMU_API ~Camera() = default;

		Vector2D<float> m_offset;
		Vector2D<float> m_size;
		int m_pixelsPerUnit;
		bool m_clampingOn; 
		Vector2D<int> m_bounds;
	};
}
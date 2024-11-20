#pragma once

#include "../Core.h"
#include "../ECS/Component.h"
#include "../MathUtil.h"
#include "../Physics/BodyTypes.h"
#include "../GameState.h"

struct b2BodyId;
struct b2ShapeId;
struct b2WorldId;

namespace Engine
{
	struct PhysicsBody : public Component
	{
		EMU_API PhysicsBody(Entity* ptrEntity) :
			m_bodyId(nullptr), m_shapeId(nullptr), m_worldId(nullptr),
			m_bodyType(STATIC), m_dimensions(Vector2D<float>(1.0f, 1.0f)),
			m_halfDimensions(Vector2D<float>(0.5f, 0.5f)), m_startingPosition(Vector2D<float>(1.0f, 1.0f)),
			m_gravityOn(true), Component(ptrEntity) {}

		b2BodyId* m_bodyId;
		b2ShapeId* m_shapeId;
		b2WorldId* m_worldId;

		BodyType m_bodyType;

		Vector2D<float> m_dimensions;
		Vector2D<float> m_halfDimensions;
		Vector2D<float> m_startingPosition;

		bool m_gravityOn;

		// Temp
		EMU_API void OnDeactivate() override;
		EMU_API void OnActivate() override;
	};
}

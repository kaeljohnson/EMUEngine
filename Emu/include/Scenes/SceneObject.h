#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../Core.h"
#include "../MathUtil.h"

#include "../Physics/PhysicsBody.h"
#include "../Events/Event.h"

namespace Engine
{
	// SceneObject class. Currently behaves as a wrapper for a physics body and a texture.

	class SceneObject
	{
	public:
		bool Enabled;
		bool Visible;
		int LayerIdx;
		int DirectionFacing;
	public:
		SceneObject(const BodyType bodyType, const bool fixed, const Vector2D<float> position, const Vector2D<float> size, int directionFacing, const bool visible);

		EMU_API virtual ~SceneObject() = default;

		EMU_API inline std::shared_ptr<PhysicsBody> GetPhysicsBody() { return m_physicsBody; }
		EMU_API inline size_t GetUUID() const { return uuid; }

	public:
		void EngineSideUpdate();
		void UpdatePrevPosition();

		virtual void Update();

	protected:
		std::shared_ptr<PhysicsBody> m_physicsBody;
		size_t uuid;
	};
}
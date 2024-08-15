#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../Core.h"
#include "../MathUtil.h"

#include "../Physics/PhysicsBody.h"
#include "../Textures/ITexture.h"
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
		SceneObject(const BodyType bodyType, const bool fixed, const Math::Vector2D<float> position, const Math::Vector2D<float> size, int directionFacing);

		EMU_API virtual ~SceneObject() = default;

		// Should clients be able to get the physics body?
		EMU_API inline std::shared_ptr<PhysicsBody> GetPhysicsBody() { return m_physicsBody; }
		EMU_API inline std::string GetUUID() const { return uuid; }

	public:
		void EngineSideUpdate();
		void UpdatePrevPosition();

		virtual void Update();

	private:
		std::string uuid;

	protected:
		std::shared_ptr<PhysicsBody> m_physicsBody;
	};
}
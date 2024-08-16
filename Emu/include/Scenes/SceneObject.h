#pragma once

#include <memory>
#include <string>

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
		SceneObject(const BodyType bodyType, const bool fixed, const Math::Vector2D<float> position, const Math::Vector2D<float> size, int directionFacing);

		EMU_API virtual ~SceneObject() = default;

		// Should clients be able to get the physics body?
		EMU_API inline std::shared_ptr<PhysicsBody> GetPhysicsBody() { return m_physicsBody; }
		EMU_API inline std::string GetUUID() const { return uuid; }
		EMU_API inline bool IsEnabled() const { return m_enabled; }
		EMU_API inline bool IsVisible() const { return m_visible; }
		EMU_API inline int GetLayerIdx() const { return m_layerIdx; }
		EMU_API inline int GetDirectionFacing() const { return m_directionFacing; }

		EMU_API inline void SetDirectionFacing(int directionFacing);
		EMU_API inline void SetLayerIdx(int layerIdx);
		EMU_API inline void SetEnabled(bool enabled);
		EMU_API inline void SetVisible(bool visible);

	public:
		void EngineSideUpdate();
		void UpdatePrevPosition();

		virtual void Update();

	private:
		std::string uuid;

	protected:
		std::shared_ptr<PhysicsBody> m_physicsBody;
		bool m_enabled;
		bool m_visible;
		int m_layerIdx;
		int m_directionFacing;
	};
}
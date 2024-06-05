#pragma once

#include <memory>
#include <unordered_map>

#include "../Core.h"

#include "../Events/EventListener.h"
#include "../Events/Event.h"
#include "../Scenes/SceneObject.h"
#include "../Textures/Texture.h"

namespace Engine
{
	class Controllable : public SceneObject, public EventListener
	{
	public:
		EMU_API Controllable(const BodyType bodyType, const bool fixed, const float startingXInMeters,
			const float startingYInMeters, const float widthInMeters, const float heightInMeters, Texture& ptrTexture);
		EMU_API virtual ~Controllable() = default;
		EMU_API virtual void Update() override;
		EMU_API virtual void Jump();

	protected:
		const float m_xSwitchDeceleration;
		const float m_xDeceleration;
		const float m_yDeceleration;
		const float m_xAcceleration;
		const float m_yAcceleration;
		const float m_xMaxVelocity;
		const float m_yMaxVelocity;

		const float m_maxJumpCharge;
		const float m_jumpChargeIncrement;
		const float m_jumpForce;
		const float m_minJumpForce;

		bool m_isJumping;
		float m_jumpCharge;

		const std::unordered_map<EventType, bool>& refKeyStates;
	};
}
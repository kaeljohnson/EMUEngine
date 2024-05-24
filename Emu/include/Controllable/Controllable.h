#pragma once

#include <memory>
#include <unordered_map>

#include "../Core.h"

#include "../Events/EventListener.h"
#include "../Physics/IPhysicsBody.h"
#include "../Textures/Texture.h"
#include "../Events/Event.h"
#include "../Scenes/SceneObject.h"

namespace Engine
{
	class Controllable : public EventListener, public SceneObject
	{
	private:
		float m_xVelocity;
		float m_yVelocity;

		const float XSWITCHDECELERATION = 70.0f;
		const float XDECELERATION = 5.0f;
		const float XACCELERATION = 30.0f;
		const float MAX_XVELOCITY = 30.0f;
		float m_xAcceleration;

		const float YACCELERATION = 30.0f;
		const float MAX_YVELOCITY = 60.0f;
		float m_yAcceleration;

		float m_jumpForce;

		// Jumping state will need to be handled 
		// using the box2d callback system.
		bool m_isJumping = false;

		std::unordered_map<EventType, bool> m_keyStates;

	public:
		EMU_API Controllable(std::shared_ptr<IPhysicsBody> ptrPhysicsBody, Texture* ptrTexture);
		~Controllable() = default;

		EMU_API virtual void ProcessEvent(Event& e) override;

		EMU_API void SetGravity(bool enabled);

		EMU_API void SetXVelocity(const float xVel);
		EMU_API void SetYVelocity(const float yVel);
		EMU_API const float GetXVelocity() const;
		EMU_API const float GetYVelocity() const;

		EMU_API virtual void update() override;

		EMU_API virtual void Jump();
	};
}
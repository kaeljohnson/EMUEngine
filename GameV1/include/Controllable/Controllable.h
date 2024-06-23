#pragma once

#include <memory>
#include <unordered_map>

#include <Engine.h>

class Controllable : public Engine::Entity, public Engine::EventListener
{
public:
	Controllable(const float startingXInMeters, const float startingYInMeters,
		const float widthInMeters, const float heightInMeters, std::shared_ptr<Engine::ITexture> ptrTexture);
	~Controllable() = default;
	void Update() override;
	void Jump(bool canJump);

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

	bool m_jumpKeyHeld;
	bool m_canJump;
	bool m_isJumping;
	float m_jumpCharge;

	float m_coyoteTime;
	const float m_coyoteTimeDuration;

	Engine::EventType m_jumpKeyDown;
	Engine::EventType m_jumpKeyUp;

	Engine::EventType m_moveLeftKeyDown;
	Engine::EventType m_moveLeftKeyUp;
	Engine::EventType m_moveRightKeyDown;
	Engine::EventType m_moveRightKeyUp;

private:
	void checkForJump(bool onGround);
};

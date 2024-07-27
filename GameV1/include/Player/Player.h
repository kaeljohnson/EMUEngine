#pragma once

#include <memory>
#include <unordered_map>

#include <Engine.h>

enum PlayerState
{
	Idle,
	HorizontalMovement,
	Jumping,
	Falling,
};

class Player : public Engine::Entity, public Engine::EventListener
{
public:
	Player(const float startingXInMeters, const float startingYInMeters,
		const float widthInMeters, const float heightInMeters, std::shared_ptr<Engine::ITexture> ptrTexture);
	~Player() = default;
	void Update() override;
	void UpdateHorizontalMovement(std::pair<float, float>& force, const float currentVelocityX);
	void UpdateVerticalMovement();
	void Jump();
	void ContinueJump();
	void EndJump();
	void Dash(bool canDash);

protected:
	bool m_jumpKeyHeld;
	bool m_canJump;
	bool m_isJumping;
	bool m_isFalling;
	float m_jumpCharge;

	bool m_isDashing;

	float m_coyoteTime;

	bool m_endJump;
	bool m_onGround;

	Engine::EventType m_jumpKeyDown;
	Engine::EventType m_jumpKeyUp;

	Engine::EventType m_moveLeftKeyDown;
	Engine::EventType m_moveLeftKeyUp;
	Engine::EventType m_moveRightKeyDown;
	Engine::EventType m_moveRightKeyUp;

private:
	// void checkForJump(bool onGround);

	PlayerState m_currentState;

	void UpdateMovement(std::pair<float, float>& force, const float currentVelocityX);
	void TransitionToState(PlayerState newState);

	void beginIdle();
	void updateIdle();
	void endIdle();

	void beginWalk(std::pair<float, float>& force, const float currentVelocityX);
	void updateWalk(std::pair<float, float>& force, const float currentVelocityX);
	void endWalk(std::pair<float, float>& force, const float currentVelocityX);

	void beginJump();
	void updateJump();
	void endJump();
};

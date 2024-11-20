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

enum PlayerDirection
{
	Left,
	Right,
};

class Player
{
public:
	Player(Engine::Entity* entity, const float startingX, const float startingY,
		const float width, const float height);
	~Player() = default;
	void Update();

	PlayerState m_currentState;
	PlayerDirection m_currentDirection;

private:
	Engine::Entity* m_ptrEntity;

	bool m_canJump;
	float m_jumpCharge;
	float m_coyoteTime;
	bool m_onGround;
	Engine::Vector2D<float> m_force;

	const Engine::EventStatesMap& refKeyStates;

	Engine::EventType m_jumpKeyDown;
	Engine::EventType m_jumpKeyUp;

	Engine::EventType m_moveLeftKeyDown;
	Engine::EventType m_moveLeftKeyUp;
	Engine::EventType m_moveRightKeyDown;
	Engine::EventType m_moveRightKeyUp;

private:
	void UpdateMovement();
	void TransitionToState(PlayerState newState);

	void beginIdle();
	void updateIdle();
	void endIdle();

	void startHorizontalMove();
	void updateHorizontalMove();
	void endHorizontalMove();


	void beginJump();
	void updateJump();
	void endJump();
};

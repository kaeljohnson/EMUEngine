#pragma once

#include <memory>
#include <unordered_map>

#include <Engine.h>

struct PlayerContactListener : public Engine::SingleEntityContactListener
{
	PlayerContactListener(Engine::Entity playerEntity)
		: Engine::SingleEntityContactListener(playerEntity) {}

	void OnContactBegin(const Engine::Contact event) override
	{
		CLIENT_INFO_D("Player contact Begin");
	}

	void OnContactEnd(const Engine::Contact event) override
	{
		CLIENT_INFO_D("Player contact End");
	}
};

struct PlayerSensorListener : public Engine::SingleEntitySensorListener
{
	PlayerSensorListener(Engine::Entity playerEntity)
		: Engine::SingleEntitySensorListener(playerEntity) {}

	void OnContactBegin(const Engine::Contact event) override
	{
		CLIENT_INFO_D("Player sensor Begin");
	}

	void OnContactEnd(const Engine::Contact event) override
	{
		CLIENT_INFO_D("Player sensor End");
	}
};

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
	Player();
	~Player() = default;
	void Update(Engine::Entity entity);
	void OnBeginContact(Engine::BeginContact beginContact);
	void OnEndContact(Engine::EndContact ptrEntity);
	void OnBeginSensing(Engine::BeginSensing beginSensing);
	void OnEndSensing(Engine::EndSensing endSensing);

	PlayerState m_currentState;
	PlayerDirection m_currentDirection;

private:

	Engine::PhysicsInterface& m_refPhysicsInterface;
	Engine::TransformInterface& m_refTransformInterface;

	bool m_canJump;
	float m_jumpCharge;
	float m_coyoteTime;
	bool m_onGround;
	Engine::Vector2D<float> m_force;

	const Engine::IOEventStatesMap& refKeyStates;

	Engine::IOEventType m_jumpKeyDown;
	Engine::IOEventType m_jumpKeyUp;

	Engine::IOEventType m_moveLeftKeyDown;
	Engine::IOEventType m_moveLeftKeyUp;
	Engine::IOEventType m_moveRightKeyDown;
	Engine::IOEventType m_moveRightKeyUp;

private:
	void UpdateMovement(Engine::Entity entity);
	void TransitionToState(Engine::Entity entity, PlayerState newState);

	void beginIdle();
	void updateIdle();
	void endIdle();

	void startHorizontalMove();
	void updateHorizontalMove();
	void endHorizontalMove(Engine::Entity entity);


	void beginJump();
	void updateJump();
	void endJump();
};

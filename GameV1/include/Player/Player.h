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
		const float width, const float height, const Engine::EventStatesMap& keyStates);
	~Player() = default;
	void Update();

	PlayerState m_currentState;
	PlayerDirection m_currentDirection;

	// Engine::Transform* m_transform;
	// Engine::PhysicsBody* m_physicsBody;
	// Engine::Updatable* m_updatable;

private:
	const Engine::EntityID m_entityID;

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
	void UpdateMovement(Engine::PhysicsBody* physicsBodyComponent);
	void TransitionToState(PlayerState newState, Engine::PhysicsBody* physicsBodyComponent);

	void beginIdle();
	void updateIdle();
	void endIdle();

	void startHorizontalMove(Engine::PhysicsBody* physicsBodyComponent);
	void updateHorizontalMove(Engine::PhysicsBody* physicsBodyComponent);
	void endHorizontalMove(Engine::PhysicsBody* physicsBodyComponent);


	void beginJump();
	void updateJump();
	void endJump();
};

#pragma once

#include <memory>

#include <Engine.h>

#include "../../include/Player/Player.h"
#include "../../include/Player/PlayerConfig.h"

    Player::Player(const size_t id, const float startingX, const float startingY,
        const float width, const float height, const Engine::EventStatesMap& keyStates)
        // These need to be set by client.
        : m_entityID(id), refKeyStates(keyStates), m_coyoteTime(0.0f), m_canJump(false), m_jumpCharge(0.0f), m_onGround(false),
        m_jumpKeyDown(Engine::SPACE_KEY_DOWN), m_jumpKeyUp(Engine::SPACE_KEY_UP),
        m_moveLeftKeyDown(Engine::A_KEY_DOWN), m_moveLeftKeyUp(Engine::A_KEY_UP),
        m_moveRightKeyDown(Engine::D_KEY_DOWN), m_moveRightKeyUp(Engine::D_KEY_UP),
        m_currentState(PlayerState::Idle), m_currentDirection(PlayerDirection::Right)
    {
        // Need to have them be able to set this during construction of physics body.
    	// m_physicsBody->SetStartingFriction(0.0f);
        Engine::EntityManager::AddComponent<Engine::Transform>(id,
            Engine::Vector2D(startingX, startingY), Engine::Vector2D(width, height), 1.0f, 1.0f, 1.0f);

        Engine::EntityManager::AddComponent<Engine::PhysicsBody>(
            id, Engine::BodyType::DYNAMIC, false, Engine::Vector2D<float>(startingX, startingY), Engine::Vector2D<float>(width, height));

		Engine::EntityManager::AddComponent<Engine::Updatable>(id, [this]() { Update(); });
    }

    void Player::Update()
    {
		Engine::PhysicsBody* physicsBodyComponent = 
            Engine::EntityManager::GetComponentManager<Engine::PhysicsBody>().GetComponent(m_entityID);

		if (physicsBodyComponent == nullptr)
		{
			ENGINE_ERROR("PhysicsBody component not found for entity: " + std::to_string(m_entityID));
			return;
		}

        m_onGround = physicsBodyComponent->GetHasCollisionBelow();

        m_force = { 0.0f, 0.0f };

        UpdateMovement(physicsBodyComponent);

        physicsBodyComponent->ApplyForceToBody(m_force);

        // Check if the current velocity is below the threshold and set it to zero
        if (std::abs(physicsBodyComponent->GetVelocity().X) < MIN_VELOCITY_THRESHOLD)
        {
            physicsBodyComponent->SetXVelocity(0.0f);
        }
        else
        {
            // Limit the velocity
            if (physicsBodyComponent->GetVelocity().X >= X_MAX_VELOCITY)
            {
                physicsBodyComponent->SetXVelocity(X_MAX_VELOCITY);
            }

            if (physicsBodyComponent->GetVelocity().X <= -X_MAX_VELOCITY)
            {
                physicsBodyComponent->SetXVelocity(-X_MAX_VELOCITY);
            }
        }
    }

    void Player::UpdateMovement(Engine::PhysicsBody* physicsBodyComponent)
    {
		Engine::Transform* transformComponent =
			Engine::EntityManager::GetComponentManager<Engine::Transform>().GetComponent(m_entityID);

        float currentVelocityX = physicsBodyComponent->GetVelocity().X;

        if (m_currentDirection == PlayerDirection::Right) transformComponent->DirectionFacing = 1;
		else transformComponent->DirectionFacing = -1;

        switch (m_currentState)
        {
        case PlayerState::Idle:
            // Transition to HorizontalMovement if movement keys are pressed
            if (refKeyStates.at(m_moveRightKeyDown) && refKeyStates.at(m_moveLeftKeyUp))
            {
                m_currentDirection = PlayerDirection::Right;
                TransitionToState(PlayerState::HorizontalMovement, physicsBodyComponent);
                startHorizontalMove(physicsBodyComponent);
            }
            else if (refKeyStates.at(m_moveLeftKeyDown) && refKeyStates.at(m_moveRightKeyUp))
            {
                m_currentDirection = PlayerDirection::Left;
                TransitionToState(PlayerState::HorizontalMovement, physicsBodyComponent);
                startHorizontalMove(physicsBodyComponent);
            }
            else
            {
				// Apply deceleration when no movement keys are pressed
				endHorizontalMove(physicsBodyComponent);
			}
            // Transition to Jumping if jump key is pressed
            if (refKeyStates.at(m_jumpKeyDown) && m_canJump)
            {
                TransitionToState(PlayerState::Jumping, physicsBodyComponent);
            }
            break;

        case PlayerState::HorizontalMovement:

            // Horizontal Movement
            if (refKeyStates.at(m_moveRightKeyDown) && refKeyStates.at(m_moveLeftKeyUp))
            {
                m_currentDirection = PlayerDirection::Right;
                updateHorizontalMove(physicsBodyComponent);
            }
            else if (refKeyStates.at(m_moveLeftKeyDown) && refKeyStates.at(m_moveRightKeyUp))
            {
                m_currentDirection = PlayerDirection::Left;
                updateHorizontalMove(physicsBodyComponent);
            }
            else
            {
                // Apply deceleration when no movement keys are pressed
                endHorizontalMove(physicsBodyComponent);
                m_force.X = -currentVelocityX * X_DECELERATION * physicsBodyComponent->GetSize();
                if (m_onGround && std::abs(currentVelocityX) < MIN_VELOCITY_THRESHOLD)
                {
                    TransitionToState(PlayerState::Idle, physicsBodyComponent);
                }
            }

            // Vertical Movement
            if (refKeyStates.at(m_jumpKeyDown) && m_canJump)
            {
                TransitionToState(PlayerState::Jumping, physicsBodyComponent);
            }
            else if (!m_onGround)
            {
                TransitionToState(PlayerState::Falling, physicsBodyComponent);
            }
            break;

        case PlayerState::Jumping:

            // Allow horizontal movement while jumping
            if (refKeyStates.at(m_moveRightKeyDown) && refKeyStates.at(m_moveLeftKeyUp))
            {
                m_currentDirection = PlayerDirection::Right;
                updateHorizontalMove(physicsBodyComponent);
            }
            else if (refKeyStates.at(m_moveLeftKeyDown) && refKeyStates.at(m_moveRightKeyUp))
            {
                m_currentDirection = PlayerDirection::Left;
                updateHorizontalMove(physicsBodyComponent);
            }
            else
            {
                // Apply deceleration when no movement keys are pressed
                endHorizontalMove(physicsBodyComponent);
            }

            if (refKeyStates.at(m_jumpKeyUp) || physicsBodyComponent->GetVelocity().Y > 0 || physicsBodyComponent->GetHasCollisionAbove())
            {
                TransitionToState(PlayerState::Falling, physicsBodyComponent);
            }
            else
            {
                if (m_jumpCharge < MAX_JUMP_CHARGE)
                {
                    m_jumpCharge += JUMP_CHARGE_INCREMENT;
                }
                m_force.Y = -JUMP_FORCE * (MAX_JUMP_CHARGE - m_jumpCharge) * physicsBodyComponent->GetSize();
            }
            break;

        case PlayerState::Falling:
            // Allow horizontal movement while falling
            bool continueMoving = false;
            if (refKeyStates.at(m_moveRightKeyDown) && refKeyStates.at(m_moveLeftKeyUp))
            {
                m_currentDirection = PlayerDirection::Right;
                // TransitionToState(PlayerState::HorizontalMovement);
                updateHorizontalMove(physicsBodyComponent);
                continueMoving = true;
            }
            else if (refKeyStates.at(m_moveLeftKeyDown) && refKeyStates.at(m_moveRightKeyUp))
            {
                m_currentDirection = PlayerDirection::Left;
                // TransitionToState(PlayerState::HorizontalMovement);
                startHorizontalMove(physicsBodyComponent);
                continueMoving = true;
            }
            else
            {
                // Apply deceleration when no movement keys are pressed
                endHorizontalMove(physicsBodyComponent);
                if (m_onGround && std::abs(currentVelocityX) < MIN_VELOCITY_THRESHOLD)
                {
                    TransitionToState(PlayerState::Idle, physicsBodyComponent);
                }
            }

            if (m_onGround && continueMoving)
            {
                updateHorizontalMove(physicsBodyComponent);
                TransitionToState(PlayerState::HorizontalMovement, physicsBodyComponent);
            }
            else if (m_onGround)
            {
				TransitionToState(PlayerState::Idle, physicsBodyComponent);
			}
            else if (refKeyStates.at(m_jumpKeyDown) && m_canJump && m_coyoteTime <= COYOTE_TIME_DURATION)
            {
				TransitionToState(PlayerState::Jumping, physicsBodyComponent);
			}

            break;
        }

        if (!m_onGround)
        {
            m_coyoteTime += TIME_STEP;
        }
        else
        {
            m_coyoteTime = 0.0f;
        }

        if (refKeyStates.at(m_jumpKeyUp))
        {
            m_canJump = true;
        }
    }

    void Player::TransitionToState(PlayerState newState, Engine::PhysicsBody* physicsBodyComponent)
    {
        switch (newState)
        {
        case PlayerState::Idle:
            m_jumpCharge = 0.0f;
            physicsBodyComponent->SetXVelocity(0.0f);
            break;

        case PlayerState::HorizontalMovement:
            break;

        case PlayerState::Jumping:
            m_canJump = false;
            physicsBodyComponent->SetYVelocity(0.0f);
            physicsBodyComponent->ApplyImpulseToBody({ 0.0f, -MIN_JUMP_FORCE * physicsBodyComponent->GetSize() });
            break;

        case PlayerState::Falling:
            m_jumpCharge = 0.0f;
            if (physicsBodyComponent->GetVelocity().Y < 0)
            {
                physicsBodyComponent->SetYVelocity(0.0f);
            }
            break;
        }

        m_currentState = newState;
    }


    void Player::startHorizontalMove(Engine::PhysicsBody* physicsBodyComponent)
    {
        // begin move right animation.
        updateHorizontalMove(physicsBodyComponent);
    }

    void Player::updateHorizontalMove(Engine::PhysicsBody* physicsBodyComponent)
    {
        if (m_currentDirection == PlayerDirection::Right)
        {
			m_force.X = X_ACCELERATION * physicsBodyComponent->GetSize();
		}
        else
        {
			m_force.X = -X_ACCELERATION * physicsBodyComponent->GetSize();
		}
    }

    void Player::endHorizontalMove(Engine::PhysicsBody* physicsBodyComponent)
    {
		m_force.X = -physicsBodyComponent->GetVelocity().X * X_DECELERATION * physicsBodyComponent->GetSize();
	}
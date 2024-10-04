#pragma once

#include <memory>

#include <Engine.h>

#include "../../include/Player/Player.h"
#include "../../include/Player/PlayerConfig.h"

    Player::Player(const size_t id, const float startingX, const float startingY,
        const float width, const float height, const Engine::EventStatesMap& keyStates)
        // These need to be set by client.
        : refKeyStates(keyStates), m_coyoteTime(0.0f), m_canJump(false), m_jumpCharge(0.0f), m_onGround(false),
        m_jumpKeyDown(Engine::SPACE_KEY_DOWN), m_jumpKeyUp(Engine::SPACE_KEY_UP),
        m_moveLeftKeyDown(Engine::A_KEY_DOWN), m_moveLeftKeyUp(Engine::A_KEY_UP),
        m_moveRightKeyDown(Engine::D_KEY_DOWN), m_moveRightKeyUp(Engine::D_KEY_UP),
        m_currentState(PlayerState::Idle), m_currentDirection(PlayerDirection::Right),
		m_transform(Engine::ComponentManagerRegistry::GetManager<Engine::Transform>().AddAndGetComponent(id)), 
        m_physicsBody(Engine::ComponentManagerRegistry::GetManager<Engine::PhysicsBody>().AddAndGetComponent(
            id, id, Engine::BodyType::DYNAMIC, false, Engine::Vector2D<float>(startingX, startingY), Engine::Vector2D<float>(width, height))),
        m_updatable(Engine::ComponentManagerRegistry::GetManager<Engine::Updatable>().AddAndGetComponent(id, id, [this]() { Update(); }))
    {
        // Need to have them be able to set this during construction of physics body.
    	m_physicsBody->SetStartingFriction(0.0f);
    }

    void Player::Update()
    {
        m_onGround = m_physicsBody->GetHasCollisionBelow();

        m_force = { 0.0f, 0.0f };

        UpdateMovement();

        m_physicsBody->ApplyForceToBody(m_force);

        // Check if the current velocity is below the threshold and set it to zero
        if (std::abs(m_physicsBody->GetVelocity().X) < MIN_VELOCITY_THRESHOLD)
        {
            m_physicsBody->SetXVelocity(0.0f);
        }
        else
        {
            // Limit the velocity
            if (m_physicsBody->GetVelocity().X >= X_MAX_VELOCITY)
            {
                m_physicsBody->SetXVelocity(X_MAX_VELOCITY);
            }

            if (m_physicsBody->GetVelocity().X <= -X_MAX_VELOCITY)
            {
                m_physicsBody->SetXVelocity(-X_MAX_VELOCITY);
            }
        }
    }

    void Player::UpdateMovement()
    {
        float currentVelocityX = m_physicsBody->GetVelocity().X;

        if (m_currentDirection == PlayerDirection::Right) m_transform->DirectionFacing = 1;
		else m_transform->DirectionFacing = -1;

        switch (m_currentState)
        {
        case PlayerState::Idle:
            // Transition to HorizontalMovement if movement keys are pressed
            if (refKeyStates.at(m_moveRightKeyDown) && refKeyStates.at(m_moveLeftKeyUp))
            {
                m_currentDirection = PlayerDirection::Right;
                TransitionToState(PlayerState::HorizontalMovement);
                startHorizontalMove();
            }
            else if (refKeyStates.at(m_moveLeftKeyDown) && refKeyStates.at(m_moveRightKeyUp))
            {
                m_currentDirection = PlayerDirection::Left;
                TransitionToState(PlayerState::HorizontalMovement);
                startHorizontalMove();
            }
            else
            {
				// Apply deceleration when no movement keys are pressed
				endHorizontalMove();
			}
            // Transition to Jumping if jump key is pressed
            if (refKeyStates.at(m_jumpKeyDown) && m_canJump)
            {
                TransitionToState(PlayerState::Jumping);
            }
            break;

        case PlayerState::HorizontalMovement:

            // Horizontal Movement
            if (refKeyStates.at(m_moveRightKeyDown) && refKeyStates.at(m_moveLeftKeyUp))
            {
                m_currentDirection = PlayerDirection::Right;
                updateHorizontalMove();
            }
            else if (refKeyStates.at(m_moveLeftKeyDown) && refKeyStates.at(m_moveRightKeyUp))
            {
                m_currentDirection = PlayerDirection::Left;
                updateHorizontalMove();
            }
            else
            {
                // Apply deceleration when no movement keys are pressed
                endHorizontalMove();
                m_force.X = -currentVelocityX * X_DECELERATION * m_physicsBody->GetSize();
                if (m_onGround && std::abs(currentVelocityX) < MIN_VELOCITY_THRESHOLD)
                {
                    TransitionToState(PlayerState::Idle);
                }
            }

            // Vertical Movement
            if (refKeyStates.at(m_jumpKeyDown) && m_canJump)
            {
                TransitionToState(PlayerState::Jumping);
            }
            else if (!m_onGround)
            {
                TransitionToState(PlayerState::Falling);
            }
            break;

        case PlayerState::Jumping:

            // Allow horizontal movement while jumping
            if (refKeyStates.at(m_moveRightKeyDown) && refKeyStates.at(m_moveLeftKeyUp))
            {
                m_currentDirection = PlayerDirection::Right;
                updateHorizontalMove();
            }
            else if (refKeyStates.at(m_moveLeftKeyDown) && refKeyStates.at(m_moveRightKeyUp))
            {
                m_currentDirection = PlayerDirection::Left;
                updateHorizontalMove();
            }
            else
            {
                // Apply deceleration when no movement keys are pressed
                endHorizontalMove();
            }

            if (refKeyStates.at(m_jumpKeyUp) || m_physicsBody->GetVelocity().Y > 0 || m_physicsBody->GetHasCollisionAbove())
            {
                TransitionToState(PlayerState::Falling);
            }
            else
            {
                if (m_jumpCharge < MAX_JUMP_CHARGE)
                {
                    m_jumpCharge += JUMP_CHARGE_INCREMENT;
                }
                m_force.Y = -JUMP_FORCE * (MAX_JUMP_CHARGE - m_jumpCharge) * m_physicsBody->GetSize();
            }
            break;

        case PlayerState::Falling:
            // Allow horizontal movement while falling
            bool continueMoving = false;
            if (refKeyStates.at(m_moveRightKeyDown) && refKeyStates.at(m_moveLeftKeyUp))
            {
                m_currentDirection = PlayerDirection::Right;
                // TransitionToState(PlayerState::HorizontalMovement);
                updateHorizontalMove();
                continueMoving = true;
            }
            else if (refKeyStates.at(m_moveLeftKeyDown) && refKeyStates.at(m_moveRightKeyUp))
            {
                m_currentDirection = PlayerDirection::Left;
                // TransitionToState(PlayerState::HorizontalMovement);
                startHorizontalMove();
                continueMoving = true;
            }
            else
            {
                // Apply deceleration when no movement keys are pressed
                endHorizontalMove();
                if (m_onGround && std::abs(currentVelocityX) < MIN_VELOCITY_THRESHOLD)
                {
                    TransitionToState(PlayerState::Idle);
                }
            }

            if (m_onGround && continueMoving)
            {
                updateHorizontalMove();
                TransitionToState(PlayerState::HorizontalMovement);
            }
            else if (m_onGround)
            {
				TransitionToState(PlayerState::Idle);
			}
            else if (refKeyStates.at(m_jumpKeyDown) && m_canJump && m_coyoteTime <= COYOTE_TIME_DURATION)
            {
				TransitionToState(PlayerState::Jumping);
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

    void Player::TransitionToState(PlayerState newState)
    {
        switch (newState)
        {
        case PlayerState::Idle:
            m_jumpCharge = 0.0f;
            m_physicsBody->SetXVelocity(0.0f);
            break;

        case PlayerState::HorizontalMovement:
            break;

        case PlayerState::Jumping:
            m_canJump = false;
            m_physicsBody->SetYVelocity(0.0f);
            m_physicsBody->ApplyImpulseToBody({ 0.0f, -MIN_JUMP_FORCE * m_physicsBody->GetSize() });
            break;

        case PlayerState::Falling:
            m_jumpCharge = 0.0f;
            if (m_physicsBody->GetVelocity().Y < 0)
            {
                m_physicsBody->SetYVelocity(0.0f);
            }
            break;
        }

        m_currentState = newState;
    }


    void Player::startHorizontalMove()
    {
        // begin move right animation.
        updateHorizontalMove();
    }

    void Player::updateHorizontalMove()
    {
        if (m_currentDirection == PlayerDirection::Right)
        {
			m_force.X = X_ACCELERATION * m_physicsBody->GetSize();
		}
        else
        {
			m_force.X = -X_ACCELERATION * m_physicsBody->GetSize();
		}
    }

    void Player::endHorizontalMove()
    {
		m_force.X = -m_physicsBody->GetVelocity().X * X_DECELERATION * m_physicsBody->GetSize();
	}
#pragma once

#include <memory>

#include <Engine.h>

#include "../../include/Player/Player.h"
#include "../../include/Player/PlayerConfig.h"

    Player::Player(const float startingXInMeters, const float startingYInMeters,
        const float widthInMeters, const float heightInMeters, std::shared_ptr<Engine::ITexture> refTexture)
        // These need to be set by client.
        : m_isJumping(false), m_coyoteTime(0.0f), m_canJump(false),
        m_jumpKeyDown(Engine::SPACE_KEY_DOWN), m_jumpKeyUp(Engine::SPACE_KEY_UP),
        m_moveLeftKeyDown(Engine::A_KEY_DOWN), m_moveLeftKeyUp(Engine::A_KEY_UP),
        m_moveRightKeyDown(Engine::D_KEY_DOWN), m_moveRightKeyUp(Engine::D_KEY_UP),
        m_jumpCharge(0.0f), m_isDashing(false), m_onGround(false), m_isFalling(false),
        m_currentState(PlayerState::Idle), m_currentDirection(PlayerDirection::Right),
        Entity(startingXInMeters, startingYInMeters, widthInMeters, heightInMeters, refTexture, 1)
    {
    	m_physicsBody->SetFriction(0.0f);
    }

    void Player::Update()
    {
        m_onGround = m_physicsBody->GetHasBottomCollision();

        std::pair<float, float> force = { 0.0f, 0.0f };
        float currentVelocityX = m_physicsBody->GetXVelocity();

        UpdateMovement(force, currentVelocityX);

        m_physicsBody->ApplyForceToBox(force);

        // Check if the current velocity is below the threshold and set it to zero
        if (std::abs(m_physicsBody->GetXVelocity()) < MIN_VELOCITY_THRESHOLD)
        {
            m_physicsBody->SetXVelocity(0.0f);
        }
        else
        {
            // Limit the velocity
            if (m_physicsBody->GetXVelocity() >= X_MAX_VELOCITY)
            {
                m_physicsBody->SetXVelocity(X_MAX_VELOCITY);
            }

            if (m_physicsBody->GetXVelocity() <= -X_MAX_VELOCITY)
            {
                m_physicsBody->SetXVelocity(-X_MAX_VELOCITY);
            }
        }
    }

    void Player::UpdateMovement(std::pair<float, float>& force, const float currentVelocityX)
    {
        const float ACCELERATIONDAMPENING = m_onGround ? 1.0f : 0.90f;

        if (m_currentDirection == PlayerDirection::Right) DirectionFacing = 1;
		else DirectionFacing = -1;

        switch (m_currentState)
        {
        case PlayerState::Idle:
            // Transition to HorizontalMovement if movement keys are pressed
            CLIENT_INFO_D("Idle");
            if (refKeyStates.at(m_moveRightKeyDown) && refKeyStates.at(m_moveLeftKeyUp))
            {
                m_currentDirection = PlayerDirection::Right;
                TransitionToState(PlayerState::HorizontalMovement);
                startHorizontalMove(force, ACCELERATIONDAMPENING);
            }
            else if (refKeyStates.at(m_moveLeftKeyDown) && refKeyStates.at(m_moveRightKeyUp))
            {
                m_currentDirection = PlayerDirection::Left;
                TransitionToState(PlayerState::HorizontalMovement);
                startHorizontalMove(force, ACCELERATIONDAMPENING);
            }
            else
            {
				// Apply deceleration when no movement keys are pressed
				endHorizontalMove(force, currentVelocityX, ACCELERATIONDAMPENING);
			}
            // Transition to Jumping if jump key is pressed
            if (refKeyStates.at(m_jumpKeyDown) && m_canJump)
            {
                TransitionToState(PlayerState::Jumping);
            }
            break;

        case PlayerState::HorizontalMovement:
            CLIENT_INFO_D("Horizontal Movement");

            // Horizontal Movement
            if (refKeyStates.at(m_moveRightKeyDown) && refKeyStates.at(m_moveLeftKeyUp))
            {
                m_currentDirection = PlayerDirection::Right;
                updateHorizontalMove(force, ACCELERATIONDAMPENING);
            }
            else if (refKeyStates.at(m_moveLeftKeyDown) && refKeyStates.at(m_moveRightKeyUp))
            {
                m_currentDirection = PlayerDirection::Left;
                updateHorizontalMove(force, ACCELERATIONDAMPENING);
            }
            else
            {
                // Apply deceleration when no movement keys are pressed
                endHorizontalMove(force, currentVelocityX, ACCELERATIONDAMPENING);
                force.first = -currentVelocityX * X_DECELERATION * ACCELERATIONDAMPENING * m_physicsBody->GetSizeInMeters();
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
            CLIENT_INFO_D("Jumping");
            // Allow horizontal movement while jumping
            if (refKeyStates.at(m_moveRightKeyDown) && refKeyStates.at(m_moveLeftKeyUp))
            {
                m_currentDirection = PlayerDirection::Right;
                updateHorizontalMove(force, ACCELERATIONDAMPENING);
            }
            else if (refKeyStates.at(m_moveLeftKeyDown) && refKeyStates.at(m_moveRightKeyUp))
            {
                m_currentDirection = PlayerDirection::Left;
                updateHorizontalMove(force, ACCELERATIONDAMPENING);
            }
            else
            {
                // Apply deceleration when no movement keys are pressed
                endHorizontalMove(force, currentVelocityX, ACCELERATIONDAMPENING);
            }

            if (refKeyStates.at(m_jumpKeyUp) || m_physicsBody->GetYVelocity() > 0)
            {
                TransitionToState(PlayerState::Falling);
            }
            else
            {
                if (m_jumpCharge < MAX_JUMP_CHARGE)
                {
                    m_jumpCharge += JUMP_CHARGE_INCREMENT;
                }
                force.second = -JUMP_FORCE * (MAX_JUMP_CHARGE - m_jumpCharge) * m_physicsBody->GetSizeInMeters();
            }
            break;

        case PlayerState::Falling:
            CLIENT_INFO_D("Falling");
            // Allow horizontal movement while falling
            bool continueMoving = false;
            if (refKeyStates.at(m_moveRightKeyDown) && refKeyStates.at(m_moveLeftKeyUp))
            {
                m_currentDirection = PlayerDirection::Right;
                // TransitionToState(PlayerState::HorizontalMovement);
                updateHorizontalMove(force, ACCELERATIONDAMPENING);
                continueMoving = true;
            }
            else if (refKeyStates.at(m_moveLeftKeyDown) && refKeyStates.at(m_moveRightKeyUp))
            {
                m_currentDirection = PlayerDirection::Left;
                // TransitionToState(PlayerState::HorizontalMovement);
                startHorizontalMove(force, ACCELERATIONDAMPENING);
                continueMoving = true;
            }
            else
            {
                // Apply deceleration when no movement keys are pressed
                endHorizontalMove(force, currentVelocityX, ACCELERATIONDAMPENING);
                if (m_onGround && std::abs(currentVelocityX) < MIN_VELOCITY_THRESHOLD)
                {
                    TransitionToState(PlayerState::Idle);
                }
            }

            if (m_onGround && continueMoving)
            {
                updateHorizontalMove(force, ACCELERATIONDAMPENING);
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
            m_physicsBody->ApplyImpulseToBox({ 0.0f, -MIN_JUMP_FORCE * m_physicsBody->GetSizeInMeters() });
            break;

        case PlayerState::Falling:
            m_jumpCharge = 0.0f;
            if (m_physicsBody->GetYVelocity() < 0)
            {
                m_physicsBody->SetYVelocity(0.0f);
            }
            break;
        }

        m_currentState = newState;
    }


    void Player::startHorizontalMove(std::pair<float, float>& force, const float ACCELERATIONDAMPENING)
    {
        // begin move right animation.
        updateHorizontalMove(force, ACCELERATIONDAMPENING);
    }

    void Player::updateHorizontalMove(std::pair<float, float>& force, const float ACCELERATIONDAMPENING)
    {
        if (m_currentDirection == PlayerDirection::Right)
        {
			force.first = X_ACCELERATION * ACCELERATIONDAMPENING * m_physicsBody->GetSizeInMeters();
		}
        else
        {
			force.first = -X_ACCELERATION * ACCELERATIONDAMPENING * m_physicsBody->GetSizeInMeters();
		}
    }

    void Player::endHorizontalMove(std::pair<float, float>& force, const float currentVelocityX, const float ACCELERATIONDAMPENING)
    {
		force.first = -currentVelocityX * X_DECELERATION * ACCELERATIONDAMPENING * m_physicsBody->GetSizeInMeters();
	}
#pragma once

#include <memory>

#include <Engine.h>

#include "../../include/Player/Player.h"
#include "../../include/Player/PlayerConfig.h"

    Player::Player()
        // These need to be set by client.
        : refKeyStates(Engine::KeyStates::GetKeyStates()), 
        m_coyoteTime(0.0f), m_canJump(false), m_jumpCharge(0.0f), m_onGround(false),
        m_jumpKeyDown(Engine::SPACE_KEY_DOWN), m_jumpKeyUp(Engine::SPACE_KEY_UP),
        m_moveLeftKeyDown(Engine::A_KEY_DOWN), m_moveLeftKeyUp(Engine::A_KEY_UP),
        m_moveRightKeyDown(Engine::D_KEY_DOWN), m_moveRightKeyUp(Engine::D_KEY_UP),
        m_currentState(PlayerState::Idle), m_currentDirection(PlayerDirection::Right),
		m_refTransformInterface(Engine::EMU::GetInstance()->ITRANSFORMS())
    {

		Engine::EMU::GetInstance()->Scenes_AddComponent<Engine::PhysicsUpdater>("Level1", 'P',
            [this](Engine::Entity entity) { Update(entity); });

		Engine::EMU::GetInstance()->Scenes_AddComponent<Engine::PhysicsUpdater>("Level2", 'P',
			[this](Engine::Entity entity) { Update(entity); });
    }

    void Player::OnBeginContact(Engine::BeginContact beginContact) 
    {
		CLIENT_CRITICAL_D("Player OnBeginContact.");
    }

    void Player::OnEndContact(Engine::EndContact endContact) 
    {
		CLIENT_CRITICAL_D("Player OnEndContact.");
    }

	void Player::OnBeginSensing(Engine::BeginSensing beginSensing)
	{
		CLIENT_CRITICAL_D("Player OnBeginSensing.");
	}

    void Player::OnEndSensing(Engine::EndSensing endSensing)
    {
        CLIENT_CRITICAL_D("Player OnEndSensing.");
    }

    void Player::Update(Engine::Entity entity)
    {
		// PHYSICS UPDATES
        m_onGround = Engine::EMU::GetInstance()->Physics_HasContactBelow(entity);

        // m_onGround = true;

        m_force = { 0.0f, 0.0f };

        UpdateMovement(entity);

        // Could remove argument or add physics interface that takes a physics body ref.

        Engine::EMU::GetInstance()->Physics_ApplyForceToBody(entity, m_force);

        // Check if the current velocity is below the threshold and set it to zero
        if (std::abs(Engine::EMU::GetInstance()->Physics_GetVelocity(entity).X) < MIN_VELOCITY_THRESHOLD)
        {
            Engine::EMU::GetInstance()->Physics_SetXVelocity(entity, 0.0f);
        }
        else
        {
            // Limit the velocity
            if (Engine::EMU::GetInstance()->Physics_GetVelocity(entity).X >= X_MAX_VELOCITY)
            {
                Engine::EMU::GetInstance()->Physics_SetXVelocity(entity, X_MAX_VELOCITY);

            }

            if (Engine::EMU::GetInstance()->Physics_GetVelocity(entity).X <= -X_MAX_VELOCITY)
            {
                Engine::EMU::GetInstance()->Physics_SetXVelocity(entity, -X_MAX_VELOCITY);
            }
        }
    }

    void Player::UpdateMovement(Engine::Entity entity)
    {
        float currentVelocityX = Engine::EMU::GetInstance()->Physics_GetVelocity(entity).X;

		Engine::TransformInterface* transformInterface = &m_refTransformInterface;

		if (m_currentDirection == PlayerDirection::Right) transformInterface->SetDirectionFacing(entity, 1);
		else transformInterface->SetDirectionFacing(entity, -1);

        switch (m_currentState)
        {
        case PlayerState::Idle:
            // Transition to HorizontalMovement if movement keys are pressed
            if (refKeyStates.at(m_moveRightKeyDown) && refKeyStates.at(m_moveLeftKeyUp))
            {
                m_currentDirection = PlayerDirection::Right;
                TransitionToState(entity, PlayerState::HorizontalMovement);
                startHorizontalMove();
            }
            else if (refKeyStates.at(m_moveLeftKeyDown) && refKeyStates.at(m_moveRightKeyUp))
            {
                m_currentDirection = PlayerDirection::Left;
                TransitionToState(entity, PlayerState::HorizontalMovement);
                startHorizontalMove();
            }
            else
            {
				// Apply deceleration when no movement keys are pressed
				endHorizontalMove(entity);
			}
            // Transition to Jumping if jump key is pressed
            if (refKeyStates.at(m_jumpKeyDown) && m_canJump)
            {
                TransitionToState(entity, PlayerState::Jumping);
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
                endHorizontalMove(entity);
                m_force.X = -currentVelocityX * X_DECELERATION;
                if (m_onGround && std::abs(currentVelocityX) < MIN_VELOCITY_THRESHOLD)
                {
                    TransitionToState(entity, PlayerState::Idle);
                }
            }

            // Vertical Movement
            if (refKeyStates.at(m_jumpKeyDown) && m_canJump)
            {
                TransitionToState(entity, PlayerState::Jumping);
            }
            else if (!m_onGround)
            {
                TransitionToState(entity, PlayerState::Falling);
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
                endHorizontalMove(entity);
            }

            if (refKeyStates.at(m_jumpKeyUp) || Engine::EMU::GetInstance()->Physics_GetVelocity(entity).Y > 0 || false/*collision above*/)
            {
                TransitionToState(entity, PlayerState::Falling);
            }
            else
            {
                if (m_jumpCharge < MAX_JUMP_CHARGE)
                {
                    m_jumpCharge += JUMP_CHARGE_INCREMENT;
                }
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
                endHorizontalMove(entity);
                if (m_onGround && std::abs(currentVelocityX) < MIN_VELOCITY_THRESHOLD)
                {
                    TransitionToState(entity, PlayerState::Idle);
                }
            }

            if (m_onGround && continueMoving)
            {
                updateHorizontalMove();
                TransitionToState(entity, PlayerState::HorizontalMovement);
            }
            else if (m_onGround)
            {
				TransitionToState(entity, PlayerState::Idle);
			}
            else if (refKeyStates.at(m_jumpKeyDown) && m_canJump && m_coyoteTime <= COYOTE_TIME_DURATION)
            {
				TransitionToState(entity, PlayerState::Jumping);
			}

            break;
        }

        if (!m_onGround)
        {
            m_coyoteTime += Engine::Time::GetTimeStep();
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

    void Player::TransitionToState(Engine::Entity entity, PlayerState newState)
    {
        switch (newState)
        {
        case PlayerState::Idle:
            m_jumpCharge = 0.0f;
            Engine::EMU::GetInstance()->Physics_SetXVelocity(entity, 0.0f);
            break;

        case PlayerState::HorizontalMovement:
            break;

        case PlayerState::Jumping:
            m_canJump = false;
            Engine::EMU::GetInstance()->Physics_SetYVelocity(entity, 0.0f);
            Engine::EMU::GetInstance()->PlaySound(0, 64); // For some reason this plays the sound multiple times if the space bar is held for even a little while.
            Engine::EMU::GetInstance()->Physics_ApplyImpulseToBody(entity, { 0.0f, -MIN_JUMP_FORCE });
            break;

        case PlayerState::Falling:
            m_jumpCharge = 0.0f;
            if (Engine::EMU::GetInstance()->Physics_GetVelocity(entity).Y < 0)
            {
                Engine::EMU::GetInstance()->Physics_SetYVelocity(entity, 0.0f);

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
			m_force.X = X_ACCELERATION;
		}
        else
        {
			m_force.X = -X_ACCELERATION;
		}
    }

    void Player::endHorizontalMove(Engine::Entity entity)
    {
		m_force.X = -Engine::EMU::GetInstance()->Physics_GetVelocity(entity).X * X_DECELERATION;
	}
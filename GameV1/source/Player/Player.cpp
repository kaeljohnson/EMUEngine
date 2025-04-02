#pragma once

#include <memory>

#include <Engine.h>

#include "../../include/Player/Player.h"
#include "../../include/Player/PlayerConfig.h"

    Player::Player(Engine::Entity entity, const float startingX, const float startingY,
        const float width, const float height)
        // These need to be set by client.
        : m_entity(entity), refKeyStates(Engine::KeyStates::GetKeyStates()), 
        m_coyoteTime(0.0f), m_canJump(false), m_jumpCharge(0.0f), m_onGround(false),
        m_jumpKeyDown(Engine::SPACE_KEY_DOWN), m_jumpKeyUp(Engine::SPACE_KEY_UP),
        m_moveLeftKeyDown(Engine::A_KEY_DOWN), m_moveLeftKeyUp(Engine::A_KEY_UP),
        m_moveRightKeyDown(Engine::D_KEY_DOWN), m_moveRightKeyUp(Engine::D_KEY_UP),
        m_currentState(PlayerState::Idle), m_currentDirection(PlayerDirection::Right)
    {
        // Need to have them be able to set this during construction of physics body.
    	// m_physicsBody->SetStartingFriction(0.0f);
        Engine::EMU::GetInstance()->AddComponent<Engine::Transform>(entity,
            Engine::Vector2D(startingX, startingY), Engine::Vector2D(width, height), 1.0f, 1.0f, 1.0f, 3);
        
        
        Engine::EMU::GetInstance()->AddComponent<Engine::PhysicsBody>(entity, Engine::BodyType::DYNAMIC, Engine::PLAYER, Engine::ALL,
			Engine::Vector2D<float>(width, height), Engine::Vector2D<float>(startingX, startingY), 0.0f, true);

        Engine::EMU::GetInstance()->AddComponent<Engine::Updatable>(entity, [this]() { Update(); });

        CLIENT_CRITICAL_D("Player Entity ID: " + std::to_string(entity));

        Engine::EMU::GetInstance()->AddComponent<Engine::SimpleContact>(entity);
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

    void Player::Update()
    {
		// PHYSICS UPDATES
        m_onGround = Engine::EMU::GetInstance()->IPHYSICS().HasContactBelow(m_entity);

        // m_onGround = true;

        m_force = { 0.0f, 0.0f };

        UpdateMovement();

        Engine::EMU::GetInstance()->IPHYSICS().ApplyForceToBody(m_entity, m_force);

        // Check if the current velocity is below the threshold and set it to zero
        if (std::abs(Engine::EMU::GetInstance()->IPHYSICS().GetVelocity(m_entity).X) < MIN_VELOCITY_THRESHOLD)
        {
            Engine::EMU::GetInstance()->IPHYSICS().SetXVelocity(m_entity, 0.0f);
        }
        else
        {
            // Limit the velocity
            if (Engine::EMU::GetInstance()->IPHYSICS().GetVelocity(m_entity).X >= X_MAX_VELOCITY)
            {
                Engine::EMU::GetInstance()->IPHYSICS().SetXVelocity(m_entity, X_MAX_VELOCITY);

            }

            if (Engine::EMU::GetInstance()->IPHYSICS().GetVelocity(m_entity).X <= -X_MAX_VELOCITY)
            {
                Engine::EMU::GetInstance()->IPHYSICS().SetXVelocity(m_entity, -X_MAX_VELOCITY);
            }
        }
    }

    void Player::UpdateMovement()
    {
        float currentVelocityX = Engine::EMU::GetInstance()->IPHYSICS().GetVelocity(m_entity).X;

		Engine::TransformInterface* transformInterface = &Engine::EMU::GetInstance()->ITRANSFORMS();

		if (m_currentDirection == PlayerDirection::Right) transformInterface->SetDirectionFacing(m_entity, 1);
		else transformInterface->SetDirectionFacing(m_entity, -1);

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
                m_force.X = -currentVelocityX * X_DECELERATION;
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

            if (refKeyStates.at(m_jumpKeyUp) || Engine::EMU::GetInstance()->IPHYSICS().GetVelocity(m_entity).Y > 0 || false/*collision above*/)
            {
                TransitionToState(PlayerState::Falling);
            }
            else
            {
                if (m_jumpCharge < MAX_JUMP_CHARGE)
                {
                    m_jumpCharge += JUMP_CHARGE_INCREMENT;
                }
                m_force.Y = -JUMP_FORCE * (MAX_JUMP_CHARGE - m_jumpCharge);
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

    void Player::TransitionToState(PlayerState newState)
    {
        switch (newState)
        {
        case PlayerState::Idle:
            m_jumpCharge = 0.0f;
            Engine::EMU::GetInstance()->IPHYSICS().SetXVelocity(m_entity, 0.0f);
            break;

        case PlayerState::HorizontalMovement:
            break;

        case PlayerState::Jumping:
            m_canJump = false;
            Engine::EMU::GetInstance()->IPHYSICS().SetYVelocity(m_entity, 0.0f);
            Engine::EMU::GetInstance()->IPHYSICS().ApplyImpulseToBody(m_entity, { 0.0f, -MIN_JUMP_FORCE });
            break;

        case PlayerState::Falling:
            m_jumpCharge = 0.0f;
            if (Engine::EMU::GetInstance()->IPHYSICS().GetVelocity(m_entity).Y < 0)
            {
                Engine::EMU::GetInstance()->IPHYSICS().SetYVelocity(m_entity, 0.0f);

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

    void Player::endHorizontalMove()
    {
		m_force.X = -Engine::EMU::GetInstance()->IPHYSICS().GetVelocity(m_entity).X * X_DECELERATION;
	}
#pragma once

#include <memory>

#include <Engine.h>

#include "../../include/Controllable/Controllable.h"

    Controllable::Controllable(const float startingXInMeters, const float startingYInMeters,
        const float widthInMeters, const float heightInMeters, std::shared_ptr<Engine::ITexture> refTexture)
        // These need to be set by client.
        : m_xSwitchDeceleration(85.0f), m_xDeceleration(10.0f), m_yDeceleration(0.0f), m_xAcceleration(30.0f), m_xMaxVelocity(30.0f),
        m_yAcceleration(30.0f), m_yMaxVelocity(60.0f), m_jumpForce(10.0f), m_jumpCharge(0.0),
        m_jumpChargeIncrement(1.0f), m_minJumpForce(15.0f), m_maxJumpCharge(10.5f), m_isJumping(false),
        m_coyoteTime(0.0f), m_coyoteTimeDuration(TIME_STEP * 2.0f), m_jumpKeyHeld(false), m_canJump(false),
        m_jumpKeyDown(Engine::SPACE_KEY_DOWN), m_jumpKeyUp(Engine::SPACE_KEY_UP),
        m_moveLeftKeyDown(Engine::A_KEY_DOWN), m_moveLeftKeyUp(Engine::A_KEY_UP),
        m_moveRightKeyDown(Engine::D_KEY_DOWN), m_moveRightKeyUp(Engine::D_KEY_UP),
        Entity(startingXInMeters, startingYInMeters, widthInMeters, heightInMeters, refTexture)
    {
    	m_physicsBody->SetFriction(0.0f);
    }

    void Controllable::Update()
    {
        bool onGround = m_physicsBody->GetHasBottomCollision();

        std::pair<float, float> force = { 0.0f, 0.0f };
        float currentVelocityX = m_physicsBody->GetXVelocity();

        // If the controllable is jumping, they should
        // have less control over their movement.        
        const float ACCELERATIONDAMPENING = onGround ? 1.0f : 0.90f;

        if (refKeyStates.at(m_moveRightKeyDown) && refKeyStates.at(m_moveLeftKeyUp))
        {
            if (currentVelocityX < 0) // If previously moving left
            {
                force = { m_xSwitchDeceleration * ACCELERATIONDAMPENING * m_physicsBody->GetSizeInMeters(), 0.0f}; // Apply larger force to the right
            }
            else // If already moving right or not moving at all
            {
                force = { m_xAcceleration * ACCELERATIONDAMPENING * m_physicsBody->GetSizeInMeters(), 0.0f }; // Apply normal force to the right
            }
        }
        else if (refKeyStates.at(m_moveLeftKeyDown) && refKeyStates.at(m_moveRightKeyUp))
        {
            if (currentVelocityX > 0) // If previously moving right
            {
                force = { -m_xSwitchDeceleration * ACCELERATIONDAMPENING * m_physicsBody->GetSizeInMeters(), 0.0f }; // Apply larger force to the left
            }
            else // If already moving left or not moving at all
            {
                force = { -m_xAcceleration * ACCELERATIONDAMPENING * m_physicsBody->GetSizeInMeters(), 0.0f }; // Apply normal force to the left
            }
        }
        else
        {
            // Apply deceleration when no keys are pressed
            force = { -currentVelocityX * m_xDeceleration * ACCELERATIONDAMPENING * m_physicsBody->GetSizeInMeters(), 0.0f };
        }

        checkForJump(onGround);

        m_physicsBody->ApplyForceToBox(force);

        // Limit the velocity
        if (m_physicsBody->GetXVelocity() > m_xMaxVelocity)
        {
			m_physicsBody->SetXVelocity(m_xMaxVelocity);
		}

        if (m_physicsBody->GetXVelocity() < -m_xMaxVelocity)
        {
            m_physicsBody->SetXVelocity(-m_xMaxVelocity);
        }
    }

    void Controllable::checkForJump(bool onGround)
    {
        bool canJump = (onGround || m_coyoteTime < m_coyoteTimeDuration) && !m_jumpKeyHeld;

        if (!onGround)
        {
            m_coyoteTime += TIME_STEP;
        }
        else
        {
            m_coyoteTime = 0.0f; // Reset coyote time if on ground
            m_isJumping = false;
        }

        if (refKeyStates.at(m_jumpKeyDown))
        {
            Jump(canJump);
            m_jumpKeyHeld = true;
        }

        if (refKeyStates.at(m_jumpKeyUp))
        {
            m_jumpCharge = 0.0f;
            m_jumpKeyHeld = false;
        }
	}

    void Controllable::Jump(bool canJump)
    {
        if (canJump)
        {
            m_coyoteTime = m_coyoteTimeDuration;

            m_isJumping = true;
            m_physicsBody->SetYVelocity(0.0f);
			m_physicsBody->ApplyImpulseToBox({ 0.0f, -m_minJumpForce * m_physicsBody->GetSizeInMeters() });
		}
        else
        {
            if (m_jumpCharge < m_maxJumpCharge)
                m_jumpCharge += m_jumpChargeIncrement;

            m_physicsBody->ApplyForceToBox({ 0.0f, -m_jumpForce * (m_maxJumpCharge - m_jumpCharge) * m_physicsBody->GetSizeInMeters() });
        }
	}
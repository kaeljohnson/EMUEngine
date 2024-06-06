#pragma once

#include <memory>

#include "../../include/Events/EventManager.h"
#include "../../include/Controllable/Controllable.h"
#include "../../include/Textures/ITexture.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
    Controllable::Controllable(const BodyType bodyType, const bool fixed, const float startingXInMeters, const float startingYInMeters,
        const float widthInMeters, const float heightInMeters, std::shared_ptr<ITexture> refTexture)
        // These need to be set by client.
        : m_xSwitchDeceleration(85.0f), m_xDeceleration(10.0f), m_yDeceleration(0.0f), m_xAcceleration(30.0f), m_xMaxVelocity(30.0f),
        m_yAcceleration(30.0f), m_yMaxVelocity(60.0f), m_jumpForce(20.0f), m_jumpCharge(0.0),
        m_jumpChargeIncrement(1.0f), m_minJumpForce(20.0f), m_maxJumpCharge(10.0f), m_isJumping(false),
        refKeyStates(EventManager::GetInstance()->GetKeyStates()),
        SceneObject(bodyType, fixed, startingXInMeters, startingYInMeters, widthInMeters, heightInMeters, refTexture)
    {
    	m_physicsBody->SetFriction(0.0f);
    }

    void Controllable::Update()
    {
        std::pair<float, float> force = { 0.0f, 0.0f };
        float currentVelocityX = m_physicsBody->GetXVelocity();

        // If the controllable is jumping, they should
        // have less control over their movement.        
        const float ACCELERATIONDAMPENING = m_physicsBody->OnGround() ? 1.0f : 0.90f;

        if (refKeyStates.at(D_KEY_DOWN) && refKeyStates.at(A_KEY_UP))
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
        else if (refKeyStates.at(A_KEY_DOWN) && refKeyStates.at(D_KEY_UP))
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

        // Jump
        if (refKeyStates.at(SPACE_KEY_DOWN))
        {
            Jump();
		}

        if (refKeyStates.at(SPACE_KEY_UP))
        {
			m_jumpCharge = 0.0f;
			m_isJumping = false;
		}

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

    void Controllable::Jump()
    {
        if (m_isJumping)
        {
            if (m_jumpCharge < m_maxJumpCharge)
                m_jumpCharge += m_jumpChargeIncrement;

            m_physicsBody->ApplyForceToBox({ 0.0f, -m_jumpForce * (m_maxJumpCharge - m_jumpCharge) * m_physicsBody->GetSizeInMeters()});
        }
        else if (m_physicsBody->OnGround())
        {
            m_isJumping = true;
            m_physicsBody->SetYVelocity(0.0f);
			m_physicsBody->ApplyImpulseToBox({ 0.0f, -m_minJumpForce * m_physicsBody->GetSizeInMeters() });
		}
	}
}
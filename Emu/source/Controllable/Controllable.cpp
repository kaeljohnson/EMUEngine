#pragma once

#include <memory>

#include "../../include/Events/EventManager.h"
#include "../../include/Controllable/Controllable.h"
#include "../../include/Textures/Texture.h"
#include "../../include/Physics/IPhysicsBody.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
    Controllable::Controllable(const BodyType bodyType, const bool fixed, const float startingXInMeters, const float startingYInMeters,
        const float widthInMeters, const float heightInMeters, Texture& refTexture)
        : XSWITCHDECELERATION(85.0f), XDECELERATION(10.0f), XACCELERATION(30.0f), MAX_XVELOCITY(30.0f),
        YACCELERATION(30.0f), MAX_YVELOCITY(60.0f), JUMPFORCE(20.0f), m_jumpCharge(0.0),
        JUMPCHARGEINCREMENT(1.0f), MINJUMPFORCE(20.0f), MAXJUMPCHARGE(10.0f), m_isJumping(false),
        refKeyStates(EventManager::GetInstance()->GetKeyStates()),
        SceneObject(bodyType, fixed, startingXInMeters, startingYInMeters, widthInMeters, heightInMeters, refTexture)
    {
    	m_physicsBody->SetFriction(0.0f);
    }

	void Controllable::SetXVelocity(const float xVel)
	{
		m_physicsBody->SetXVelocity(xVel);
	}

	void Controllable::SetYVelocity(const float yVel)
	{
		m_physicsBody->SetYVelocity(yVel);
	}

	void Controllable::SetGravity(bool enabled)
	{
		m_physicsBody->SetGravity(enabled);
	}

    const float Controllable::GetXVelocity() const
    {
		return m_physicsBody->GetXVelocity();
	}

    const float Controllable::GetYVelocity() const
    {
		return m_physicsBody->GetYVelocity();
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
                force = { XSWITCHDECELERATION * ACCELERATIONDAMPENING * m_physicsBody->GetSizeInMeters(), 0.0f}; // Apply larger force to the right
            }
            else // If already moving right or not moving at all
            {
                force = { XACCELERATION * ACCELERATIONDAMPENING * m_physicsBody->GetSizeInMeters(), 0.0f }; // Apply normal force to the right
            }
        }
        else if (refKeyStates.at(A_KEY_DOWN) && refKeyStates.at(D_KEY_UP))
        {
            if (currentVelocityX > 0) // If previously moving right
            {
                force = { -XSWITCHDECELERATION * ACCELERATIONDAMPENING * m_physicsBody->GetSizeInMeters(), 0.0f }; // Apply larger force to the left
            }
            else // If already moving left or not moving at all
            {
                force = { -XACCELERATION * ACCELERATIONDAMPENING * m_physicsBody->GetSizeInMeters(), 0.0f }; // Apply normal force to the left
            }
        }
        else
        {
            // Apply deceleration when no keys are pressed
            force = { -currentVelocityX * XDECELERATION * ACCELERATIONDAMPENING * m_physicsBody->GetSizeInMeters(), 0.0f };
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
        if (m_physicsBody->GetXVelocity() > MAX_XVELOCITY)
        {
			m_physicsBody->SetXVelocity(MAX_XVELOCITY);
		}

        if (m_physicsBody->GetXVelocity() < -MAX_XVELOCITY)
        {
            m_physicsBody->SetXVelocity(-MAX_XVELOCITY);
        }
    }

    void Controllable::Jump()
    {
        if (m_isJumping)
        {
            if (m_jumpCharge < MAXJUMPCHARGE)
                m_jumpCharge += JUMPCHARGEINCREMENT;

            m_physicsBody->ApplyForceToBox({ 0.0f, -JUMPFORCE * (MAXJUMPCHARGE - m_jumpCharge) * m_physicsBody->GetSizeInMeters()});
        }
        else if (m_physicsBody->OnGround())
        {
            m_isJumping = true;
            m_physicsBody->SetYVelocity(0.0f);
			m_physicsBody->ApplyImpulseToBox({ 0.0f, -MINJUMPFORCE * m_physicsBody->GetSizeInMeters() });
		}
	}
}
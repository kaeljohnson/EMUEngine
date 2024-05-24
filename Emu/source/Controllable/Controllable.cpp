#pragma once

#include <memory>

#include "../../include/Controllable/Controllable.h"
#include "../../include/Textures/Texture.h"
#include "../../include/Physics/IPhysicsBody.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	Controllable::Controllable(std::shared_ptr<IPhysicsBody> ptrPhysicsBody, Texture* ptrTexture) 
		: XSWITCHDECELERATION(70.0f), XDECELERATION(5.0f), XACCELERATION(30.0f), MAX_XVELOCITY(30.0f), 
        YACCELERATION(30.0f), MAX_YVELOCITY(60.0f), JUMPFORCE(30.0f), m_keyStates(),
        SceneObject(ptrPhysicsBody, ptrTexture) {}

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

	void Controllable::ProcessEvent(Event& e)
	{
        if (e.Type == D_KEY_DOWN || e.Type == A_KEY_DOWN)
        {
            m_keyStates[e.Type] = true;
            e.Handled = true;
        }
        
        if (e.Type == D_KEY_UP)
        {
            m_keyStates[D_KEY_DOWN] = false;
            e.Handled = true;
        }

        if (e.Type == A_KEY_UP)
        {
			m_keyStates[A_KEY_DOWN] = false;
			e.Handled = true;
        }

        if (e.Type == SPACE_KEY_DOWN)
        {
			m_keyStates[SPACE_KEY_DOWN] = true;
			e.Handled = true;
		}
	}

    void Controllable::update()
    {
        std::pair<float, float> force = { 0.0f, 0.0f };
        float currentVelocityX = m_physicsBody->GetXVelocity();

        if (m_keyStates[D_KEY_DOWN] && !m_keyStates[A_KEY_DOWN])
        {
            if (currentVelocityX < 0) // If previously moving left
            {
                force = { XSWITCHDECELERATION, 0.0f }; // Apply larger force to the right
            }
            else // If already moving right or not moving at all
            {
                force = { XACCELERATION, 0.0f }; // Apply normal force to the right
            }
        }
        else if (m_keyStates[A_KEY_DOWN] && !m_keyStates[D_KEY_DOWN])
        {
            if (currentVelocityX > 0) // If previously moving right
            {
                force = { -XSWITCHDECELERATION, 0.0f }; // Apply larger force to the left
            }
            else // If already moving left or not moving at all
            {
                force = { -XACCELERATION, 0.0f }; // Apply normal force to the left
            }
        }
        else
        {
            // Apply deceleration when no keys are pressed
            force = { -currentVelocityX * XDECELERATION, 0.0f };
        }

        // Jump
        if (m_keyStates[SPACE_KEY_DOWN])
        {
			Jump();
			m_keyStates[SPACE_KEY_DOWN] = false;
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
        if (!m_isJumping)
        {
            m_physicsBody->SetYVelocity(0.0f);
			m_physicsBody->ApplyImpulseToBox({ 0.0f, -JUMPFORCE });
			// m_isJumping = true;
		}
	}
}
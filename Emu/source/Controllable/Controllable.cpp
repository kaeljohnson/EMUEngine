#pragma once

#include <memory>

#include "../../include/Controllable/Controllable.h"
#include "../../include/Textures/Texture.h"
#include "../../include/Physics/IPhysicsBody.h"

namespace Engine
{
	Controllable::Controllable(std::shared_ptr<IPhysicsBody> ptrPhysicsBody, Texture* ptrTexture) 
		: m_ptrPhysicsBody(ptrPhysicsBody), SceneObject(ptrPhysicsBody, ptrTexture)
	{}

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

	void Controllable::ProcessEvent(Event& e)
	{

		// Need interface for client to do this.
        e.Handled = true;
        if (e.Type == Engine::D_KEY_DOWN)
        {
            dKeyDown = true;
            SetXVelocity(50.0f);
        }
        else if (e.Type == Engine::A_KEY_DOWN)
        {
            aKeyDown = true;
            SetXVelocity(-50.0f);
        }
        else if (e.Type == Engine::W_KEY_DOWN)
        {
            wKeyDown = true;
            SetYVelocity(-50.0f);
        }
        else if (e.Type == Engine::S_KEY_DOWN)
        {
            sKeyDown = true;
            SetYVelocity(50.0f);
        }
        else if (e.Type == Engine::D_KEY_UP)
        {
            dKeyDown = false;
            if (aKeyDown)
            {
                SetXVelocity(-50.0f);
            }
            else
            {
                SetXVelocity(0.0f);
            }
        }
        else if (e.Type == Engine::A_KEY_UP)
        {
            aKeyDown = false;
            if (dKeyDown)
            {
                SetXVelocity(50.0f);
            }
            else
            {
                SetXVelocity(0.0f);
            }
        }
        else if (e.Type == Engine::W_KEY_UP)
        {
            wKeyDown = false;
            if (sKeyDown)
            {
                SetYVelocity(50.0f);
            }
            else
            {
                SetYVelocity(0.0f);
            }
        }
        else if (e.Type == Engine::S_KEY_UP)
        {
            sKeyDown = false;
            if (wKeyDown)
            {
                SetYVelocity(-50.0f);
            }
            else
            {
                SetYVelocity(0.0f);
            }
        }
		else if (e.Type == Engine::SPACE_KEY_DOWN)
		{
			SetGravity(true);
		}
		else if (e.Type == Engine::SPACE_KEY_UP)
		{
			SetGravity(false);
		}
		else
		{
			e.Handled = false;
		}
	}
}
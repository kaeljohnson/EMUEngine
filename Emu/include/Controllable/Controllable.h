#pragma once

#include <memory>

#include "../Core.h"

#include "../Events/EventListener.h"
#include "../Physics/IPhysicsBody.h"
#include "../Textures/Texture.h"
#include "../Events/Event.h"
#include "../Scenes/SceneObject.h"

namespace Engine
{
	class Controllable : public EventListener, public SceneObject
	{
	private:
		float m_xVelocity;
		float m_yVelocity;
		std::shared_ptr<IPhysicsBody> m_ptrPhysicsBody;

		// Temporary key down variables
		// Should be replaced with a key mapping system
		// that can be defined by the client.
		bool dKeyDown = false;
		bool aKeyDown = false;
		bool wKeyDown = false;
		bool sKeyDown = false;

	public:
		EMU_API Controllable(std::shared_ptr<IPhysicsBody> ptrPhysicsBody, Texture* ptrTexture);
		~Controllable() = default;

		EMU_API virtual void ProcessEvent(Event& e) override;

		EMU_API void SetGravity(bool enabled);

		EMU_API void SetXVelocity(const float xVel);
		EMU_API void SetYVelocity(const float yVel);
	};
}
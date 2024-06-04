#pragma once

#include <memory>
#include <unordered_map>

#include "../Core.h"

#include "../Events/EventListener.h"
#include "../Events/Event.h"
#include "../Physics/IPhysicsBody.h"
#include "../Scenes/SceneObject.h"
#include "../Textures/Texture.h"

namespace Engine
{
	class Controllable : public SceneObject, public EventListener
	{
	protected:
		const float XSWITCHDECELERATION;
		const float XDECELERATION;
		const float YDECELERATION;
		const float XACCELERATION;
		const float YACCELERATION;
		const float MAX_XVELOCITY;
		const float MAX_YVELOCITY;

		const float MAXJUMPCHARGE;
		const float JUMPCHARGEINCREMENT;
		const float JUMPFORCE;
		const float MINJUMPFORCE;

		bool m_isJumping;
		float m_jumpCharge;

		const std::unordered_map<EventType, bool>& refKeyStates;

	public:
		EMU_API Controllable(const BodyType bodyType, const bool fixed, const float startingXInMeters, 
			const float startingYInMeters, const float widthInMeters, const float heightInMeters, Texture& ptrTexture);
		~Controllable() = default;

		EMU_API virtual void Update() override;
		EMU_API virtual void Jump();
	};
}
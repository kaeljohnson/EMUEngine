#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../Core.h"

#include "../Physics/IPhysicsBody.h"
#include "../Textures/ITexture.h"
#include "../Events/Event.h"

namespace Engine
{
	// SceneObject class. Currently behaves as a wrapper for a physics body and a texture.

	class SceneObject
	{
	public:
		bool Enabled;
		bool Visible;
	public:
		EMU_API SceneObject(const BodyType bodyType, const bool fixed, const float startingXInMeters,
			const float startingYInMeters, const float widthInMeters, const float heightInMeters, std::shared_ptr<ITexture> refTexture);

		EMU_API virtual ~SceneObject() = default;

		EMU_API void SetXVelocity(const float xVel);
		EMU_API void SetYVelocity(const float yVel);
		EMU_API void SetGravity(const bool gravityEnabled);

		// Should clients be able to get the physics body?
		EMU_API inline std::shared_ptr<IPhysicsBody> GetPhysicsBody() { return m_physicsBody; }
		EMU_API inline const std::shared_ptr<ITexture> GetTexture() const { return m_texture; }
		EMU_API inline const float GetXVelocity() const { return m_physicsBody->GetXVelocity(); }
		EMU_API inline const float GetYVelocity() const { return m_physicsBody->GetYVelocity(); }
		EMU_API inline std::string GetUUID() const { return uuid; }

		EMU_API virtual void Update();

	public:
		void EngineSideUpdate();
		void UpdatePrevPosition();


	private:
		std::string uuid;

	protected:
		std::shared_ptr<ITexture> m_texture;
		std::shared_ptr<IPhysicsBody> m_physicsBody;

		// Reference to the key states in the event manager. Not
		// sure if this should be here.
		const std::unordered_map<EventType, bool>& refKeyStates;
	};
}
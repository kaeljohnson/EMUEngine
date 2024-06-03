#pragma once

#include <memory>
#include <string>

#include "../Core.h"

#include "../Physics/IPhysicsBody.h"
#include "../Textures/Texture.h"

namespace Engine
{
	// SceneObject class. Currently behaves as a wrapper for a physics body and a texture.

	class SceneObject
	{
	private:
		std::string uuid;
		bool m_enabled;
		bool m_visible;

	protected:
		Texture* m_texture;
		std::shared_ptr<IPhysicsBody> m_physicsBody;

	public:
		EMU_API SceneObject(const BodyType bodyType, const bool fixed, const float startingXInMeters, 
			const float startingYInMeters, const float widthInMeters, const float heightInMeters, Texture& refTexture);
	    
		EMU_API virtual ~SceneObject() = default;

		EMU_API void SetXVelocity(const float xVel);
		EMU_API void SetYVelocity(const float yVel);
		EMU_API void SetVisibility(const bool visible);
		EMU_API std::shared_ptr<IPhysicsBody> GetPhysicsBody();
		EMU_API inline Texture* GetTexture() { return m_texture; }
		EMU_API std::string GetUUID();
		EMU_API const bool IsEnabled() const;

		void UpdatePrevPosition();

		EMU_API virtual void Update();
	};
}
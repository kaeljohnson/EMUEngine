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

	protected:
		Texture* m_texture;
		std::shared_ptr<IPhysicsBody> m_physicsBody;

	public:
		EMU_API SceneObject(std::shared_ptr<IPhysicsBody> ptrPhysicsBody, Texture* ptrTexture);
		EMU_API virtual ~SceneObject() = default;

		EMU_API std::shared_ptr<IPhysicsBody> GetPhysicsBody();
		EMU_API std::string GetUUID();
		EMU_API Texture* GetTexture();
		EMU_API const bool IsEnabled() const;

		EMU_API void SetXVelocity(const float xVel);
		EMU_API void SetYVelocity(const float yVel);
	};
}
#pragma once

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
		bool m_enabled;

	protected:
		Texture* m_texture;
		IPhysicsBody* m_physicsBody;

	public:
		EMU_API SceneObject(IPhysicsBody* ptrPhysicsBody, Texture* ptrTexture);
		EMU_API virtual ~SceneObject();

		EMU_API IPhysicsBody* GetPhysicsBody();
		EMU_API Texture* GetTexture();
		EMU_API const bool IsEnabled() const;
	};
}
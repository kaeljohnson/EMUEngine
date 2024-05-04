#pragma once

#include <string>

#include "../Core.h"

#include "../SDLWrapper/SDLWrapper.h"
#include "../Physics/IPhysicsBody.h"
#include "../Events/Event.h"
#include "../Textures/Texture.h"

namespace Engine
{
	// SceneObject class. Currently behaves as a wrapper for a physics body and a texture.

	// Will eventually be virtual
	class SceneObject
	{
	private:
		std::string m_name;
		bool m_enabled;
		bool m_attachedToScene;

	protected:
		Texture* m_texture;
		IPhysicsBody* m_physicsBody;

	public:
		EMU_API SceneObject(IPhysicsBody* ptrPhysicsBody, Texture* ptrTexture);
		EMU_API virtual ~SceneObject();

		EMU_API IPhysicsBody* GetPhysicsBody();
		EMU_API Texture* GetTexture();

		EMU_API const std::string GetName() const ;
		EMU_API const bool IsEnabled() const;
		EMU_API const bool IsAttached() const;
		EMU_API void SetAttached(bool attached);
	};
}
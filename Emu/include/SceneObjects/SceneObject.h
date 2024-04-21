#pragma once

#include "../Core.h"

#include "../SDLWrapper/SDLWrapper.h"
#include "../Physics/IPhysicsBody.h"
#include "../Events/Event.h"
#include "../Textures/Texture.h"

namespace Engine
{

	// SceneObject class. Currently behaves as a wrapper for a physics body and a texture.

	class EMU_API SceneObject
	{
	private:
		// SpriteSheet m_spriteSheet;
		// State m_state;

		Texture* m_texture;
		IPhysicsBody* m_physicsBody;

	public:
		SceneObject(IPhysicsBody* ptrPhysicsBody, Texture* ptrTexture);
		virtual ~SceneObject();

		IPhysicsBody* GetPhysicsBody();
		Texture* GetTexture();

		/*virtual void ProcessEvent(Event& e);
		virtual void Update();
		virtual void Render();
		virtual void Display();*/
	};
}
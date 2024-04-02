#pragma once

#include <sdl.h>
#include <sdl_image.h>

#include "../Events/Event.h"
#include "../Physics/Box.h"
#include "../Physics/BodyTypes.h"
#include "../Textures/Texture.h"

namespace Engine
{
	class GameObject : public Box
	{
	private:
		// Some wrapper data structure for Box2D.
		// SpriteSheet m_spriteSheet;
		// State m_state;

		float m_x;
		float m_y;

		const float m_width;
		const float m_height;

		const float m_density;
		const float m_friction;

		bool m_visible;
		bool m_collidable;
		bool m_fixed;

		Texture* m_texture;

		// Each game object will use a rect to hold the texture that needs to be rendered.
		// Box2d will handle the position of the object, and the physics.
		SDL_Rect m_rect;

	public:
		GameObject(const BodyType bodyType, const float startingX, const float startingY, const float width, const float height,
			const float density, const float friction,
			const bool visible, const bool fixed, const bool collidable, Texture* texture);
		~GameObject() = default;

		void processEvent(Event& e);
		void update();
		void render();
		void display();
		const SDL_Rect& getRect() const;

		Texture* getTexture();
	};
}
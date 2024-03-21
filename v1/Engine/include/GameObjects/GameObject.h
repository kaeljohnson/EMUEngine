#pragma once

#include <sdl.h>
#include <sdl_image.h>

#include "../Events/Event.h"

namespace Engine
{
	class GameObject
	{
	private:
		// Some wrapper data structure for Box2D.
		// SpriteSheet m_spriteSheet;
		// State m_state;

		int m_x;
		int m_y;
		bool m_visible;
		bool m_collidable;

		SDL_Rect m_rect;
		SDL_Texture* m_texture = nullptr;

	public:
		GameObject(const int startingX, const int startingY, const bool visible, const bool collidable/* Sprite Sheet, State& initialState */);
		~GameObject() = default;

		void processEvent(Event& e);
		void update();
		void render();
		void display();
	};
}
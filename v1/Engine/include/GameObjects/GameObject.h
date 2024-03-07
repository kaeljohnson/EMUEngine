#pragma once

#include <sdl.h>
#include <sdl_image.h>
#include <queue>

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
		GameObject(int startingX, int startingY, bool visible, bool collidable/* Sprite Sheet, State& initialState */);
		void free();
		
		void processEvent(Event& e);
		void update();
		void render();
		void display();
		
		~GameObject() = default;
	};
}
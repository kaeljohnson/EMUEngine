#pragma once

#include <string>

#include "../Core.h"
#include "../Events/Event.h"
#include "../GameObjects/GameObject.h"
#include "../CallbackSystem/CallbackSystem.h"

namespace Engine
{
	class EMU_API Layer
	{
	protected:
		std::string m_name;
		bool m_enabled;

		// Note: common box2d bug where collisions won't work if dynamic objects are
		// created and simulated before static objects.
		std::vector<GameObject*> m_gameObjects;

		ICallbackSystem* ptrICallbackSystem;
	public:
		Layer(std::string name);
		virtual	~Layer();

		bool isAttached;

		const std::vector<GameObject*>::iterator begin() { return m_gameObjects.begin(); }
		const std::vector<GameObject*>::iterator end() { return m_gameObjects.end(); }

		void AddToWorld();
		void RemoveFromWorld();

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void Free();
		virtual void OnUpdate();
		virtual void ProcessEvent(Event& e);

		const std::string& GetName() const;

		void AddGameObject(GameObject* gameObject);
		void RemoveGameObject(GameObject* gameObject);
	};
}
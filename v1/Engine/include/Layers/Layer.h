#pragma once

#include <string>

#include "../Events/Event.h"
#include "../GameObjects/GameObject.h"
#include "../CallbackSystem/CallbackSystem.h"

namespace Engine
{
	class Layer
	{
	protected:
		std::string m_name;
		bool m_enabled;

		// Maybe there should be two vectors, one for static objects and one for dynamic objects.
		// This would make it easy to avoid the box2d bug where if dynamic objects are added before static objects,
		// the collision detection will cause the dynamic objects to inch to the right.
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
		virtual void OnUpdate();
		virtual void ProcessEvent(Event& e);

		const std::string& GetName() const;

		void AddGameObject(GameObject* gameObject);
		void RemoveGameObject(GameObject* gameObject);
	};
}
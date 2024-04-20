#pragma once

#include <string>

#include "../Core.h"
#include "../Events/Event.h"
#include "../GameObjects/GameObject.h"
#include "../CallbackSystem/CallbackSystem.h"

namespace Engine
{
	class Layer
	{

	public:
		bool IsAttachedToScene;

	protected:
		std::string m_name;
		bool m_enabled;

		// Note: common box2d bug where collisions won't work if dynamic objects are
		// created and simulated before static objects.
		std::vector<GameObject*> m_gameObjects;

		ICallbackSystem* ptrICallbackSystem;
	public:
		EMU_API Layer(std::string name);
		EMU_API virtual	~Layer();

		const std::vector<GameObject*>::iterator begin() { return m_gameObjects.begin(); }
		const std::vector<GameObject*>::iterator end() { return m_gameObjects.end(); }

		EMU_API void AddToWorld();
		EMU_API void RemoveFromWorld();

		EMU_API virtual void OnAttach();
		EMU_API virtual void OnDetach();
		EMU_API virtual void Free();
		EMU_API virtual void OnUpdate();
		EMU_API virtual void ProcessEvent(Event& e);

		const std::string& GetName() const;

		EMU_API void AddGameObject(GameObject* gameObject);
		EMU_API void RemoveGameObject(GameObject* gameObject);
	};
}
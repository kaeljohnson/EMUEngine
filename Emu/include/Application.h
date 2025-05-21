#pragma once

#include "Includes.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Rendering/WindowRenderer.h"
#include "AssetManager.h"
#include "Events/IOEventSystem.h"
#include "Core.h"

namespace Engine
{
	class Application
	{
	public:
		Application(ECS& refECS, SceneManager& refSceneManager, IOEventSystem& refIOEventSystem);
		void Start();
		void End();

		void Activate(Entity entity);
		void Deactivate(Entity entity);

		~Application();

		// Deleted functions to ensure our app instance cannot be copied or moved.
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

	private:
		ECS& m_refECS;

		// Engine should probably own this.
		WindowRenderer m_windowRenderer;
		AssetManager m_assetManager;
		
		SceneManager& m_refSceneManager;
		Scene* m_ptrCurrentScene;
		IOEventSystem& m_refIOEventSystem;
	};
}
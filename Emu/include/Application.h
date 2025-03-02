#pragma once

#include "Includes.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Rendering/WindowRenderer.h"
#include "Core.h"

namespace Engine
{
	class Application
	{
	public:
		EMU_API Application(ECS& refECS, SceneManager& refSceneManager);
		EMU_API void Start();
		EMU_API void End();

		EMU_API ~Application();

		// Deleted functions to ensure our app instance cannot be copied or moved.
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

	private:
		ECS& m_refECS;
		WindowRenderer m_windowRenderer;
		
		SceneManager& m_sceneManager;
		std::shared_ptr<Scene> m_ptrCurrentScene;
	};
}
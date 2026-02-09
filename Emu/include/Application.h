#pragma once

#include "../Public/Includes.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Rendering/IRenderer.h"
#include "Events/IOEventSystem.h"
#include "AssetManager.h"
#include "Animations/AnimationSystem.h"
#include "Audio/AudioSystem.h"
#include "AssetManager.h"

namespace Engine
{
	/**
	* @class Application
	* 
	* @brief The main application that manages the game loop along with updating systems.
	*/
	class Application
	{
	public:
		Application(ECS& refECS);
		void Start();
		void End();

		~Application();

		// Deleted functions to ensure our app instance cannot be copied or moved.
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

	public:
		ECS& m_refECS;								/// Reference to the ECS instance.

		AssetManager m_assetManager;				/// The asset manager instance
		IRenderer m_IRenderer;						/// The renderer instance.
		SceneManager m_sceneManager;				/// The scene manager instance.
		IOEventSystem m_IOEventSystem;				/// The IO event system.
		AnimationSystem m_animationSystem;			/// The animation system.
		AudioSystem m_audioSystem;					/// The audio system.
	};
}
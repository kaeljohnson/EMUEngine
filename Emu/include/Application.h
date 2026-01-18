#pragma once

#include "Includes.h"
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
		Application(ECS& refECS, SceneManager& refSceneManager, IOEventSystem& refIOEventSystem, 
			AssetManager& refAssetManager, AudioSystem& refAudioSystem, AnimationSystem& refAnimationSystem);
		void Start();
		void End();

		~Application();

		// Deleted functions to ensure our app instance cannot be copied or moved.
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

	private:
		ECS& m_refECS;								/// Reference to the ECS instance.

		IRenderer m_IRenderer;						/// The renderer instance.
		
		SceneManager& m_refSceneManager;			/// Reference to the scene manager.
		IOEventSystem& m_refIOEventSystem;			/// Reference to the IO event system.
		AnimationSystem& m_refAnimationSystem;		/// Reference to the animation system.
		AudioSystem& m_refAudioSystem;				/// Reference to the audio system.
		AssetManager& m_refAssetManager;			/// Reference to the asset manager.
	};
}
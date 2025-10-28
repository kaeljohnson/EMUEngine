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
		ECS& m_refECS;

		IRenderer m_IRenderer;
		
		SceneManager& m_refSceneManager;
		IOEventSystem& m_refIOEventSystem;
		AnimationSystem& m_refAnimationSystem;
		AudioSystem& m_refAudioSystem;
		AssetManager& m_refAssetManager;
	};
}
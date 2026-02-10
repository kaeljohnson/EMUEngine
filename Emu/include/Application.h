#pragma once

#include "../Public/Includes.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"
#include "Rendering/IRenderer.h"
#include "Events/IOEventSystem.h"
#include "AssetManager.h"
#include "Animations/AnimationSystem.h"
#include "Animations/AnimationInterface.h"
#include "Audio/AudioSystem.h"
#include "TransformInterface.h"
#include "AssetManager.h"
#include "Camera/CameraInterface.h"
#include "ECS/ECS.h"


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
		Application(size_t numEntities);
		void Start();
		void End();

		~Application();

		// Deleted functions to ensure our app instance cannot be copied or moved.
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

	public:
		ECS m_ecs;									/// ecs

		AnimationInterface m_animationInterface;	/// The animation system.
		PhysicsInterface m_physicsInterface;		/// The physics interface.
		CameraInterface m_cameraInterface;			/// The camera interface.
		TransformInterface m_transformInterface;	/// The transform interface.

		AssetManager m_assetManager;				/// The asset manager instance.
		IRenderer m_IRenderer;						/// The renderer instance.
		SceneManager m_sceneManager;				/// The scene manager instance.
		IOEventSystem m_IOEventSystem;				/// The IO event system.
		AnimationSystem m_animationSystem;			/// The animation system.
		AudioSystem m_audioSystem;					/// The audio system.
	};
}
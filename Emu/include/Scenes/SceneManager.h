#pragma once

namespace Engine
{
	class SceneManager
	{
	private:
		static SceneManager* instance;
		SceneManager();
	public:
		static SceneManager* GetInstance();
			

		~SceneManager() = default;

		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		SceneManager& operator=(SceneManager&&) = delete;
	};
}
#pragma once

#include "../ECS/ECS.h"
#include "../AssetManager.h"

namespace Engine
{
	class AudioInterface
	{
	public:
		AudioInterface(ECS& refECS);
		~AudioInterface() = default;

		// Deleted functions to ensure our AudioInterface instance cannot be copied or moved.
		AudioInterface(const AudioInterface&) = delete;
		AudioInterface& operator=(const AudioInterface&) = delete;
		AudioInterface(AudioInterface&&) = delete;
		AudioInterface& operator=(AudioInterface&&) = delete;

	private:
		ECS& m_refECS;
	};

	class AudioSystem
	{
	public:
		AudioSystem(ECS& refECS, AssetManager& refAssetManager); // temporarily takes the AssetManager as a parameter
		~AudioSystem() = default;

		// Plays a sound by its name once. Volume is from 0 to 128 (max).
		void PlaySound(const std::string& soundName, int volume = 128, const bool loop = false);

		// Deleted functions to ensure our AudioSystem instance cannot be copied or moved.
		AudioSystem(const AudioSystem&) = delete;
		AudioSystem& operator=(const AudioSystem&) = delete;
		AudioSystem(AudioSystem&&) = delete;
		AudioSystem& operator=(AudioSystem&&) = delete;

	private:

		ECS& m_refECS;
		AssetManager& m_refAssetManager;
	};
}
#pragma once

#include "../AssetManager.h"

namespace Engine
{
	class AudioSystem
	{
	public:
		AudioSystem(ECS& refECS, AssetManager& refAssetManager); // temporarily takes the AssetManager as a parameter
		~AudioSystem() = default;

		/* 
			Plays a sound by its id / index.Volume is from 0 to 128 (max).
			arg1: soundIndex: The index of the sound to play.
			arg2: volume: The volume at which to play the sound. Default is 128 (max).
			arg3: loop: Whether to loop the sound or not. Default is false.
		*/
		void PlaySound(int soundIndex, int volume = 128, const bool loop = false);

		AudioSystem(const AudioSystem&) = delete;
		AudioSystem& operator=(const AudioSystem&) = delete;
		AudioSystem(AudioSystem&&) = delete;
		AudioSystem& operator=(AudioSystem&&) = delete;

	private:
		AssetManager& m_refAssetManager;
	};
}
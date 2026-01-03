#pragma once

#include "../AssetManager.h"

namespace Engine
{
	/**
	* @brief Audio system.
	* 
	* Handles audio playback and management.
	*/

	class AudioSystem
	{
	public:
		/**
		* @brief Constructs the audio system.
		* * @param refECS Reference to the ECS instance.
		* * @param refAssetManager Reference to the Asset Manager instance.
		*/
		AudioSystem(ECS& refECS, AssetManager& refAssetManager);
		~AudioSystem() = default;

		/** 
		* @brief Plays a sound by its id / index.Volume is from 0 to 128 (max).
		* * @param soundIndex: The index of the sound to play.
		* * @param volume: The volume at which to play the sound. Default is 128 (max).
		* * @param loop: Whether to loop the sound or not. Default is false.
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
#include "../../include/Audio/AudioSystem.h"
#include "../../include/AssetManager.h"
#include "../../include/ISDL/ISDL.h"

namespace Engine
{
	AudioSystem::AudioSystem(ECS& refECS, AssetManager& refAssetManager)
		: m_refAssetManager(refAssetManager) {}

	void AudioSystem::PlaySound(int soundIndex, int volume, const bool loop)
	{
		Mix_Chunk* ptrSound = (Mix_Chunk*)m_refAssetManager.GetSound(soundIndex);
		if (!ptrSound)
		{
			ENGINE_CRITICAL("Sound not found: {}", soundIndex);
			return;
		}

		Mix_VolumeChunk(ptrSound, volume);

		if (Mix_PlayChannel(-1, ptrSound, (loop ? -1 : 0)) == -1)
		{
			ENGINE_CRITICAL("No free channel to play sound!");
		}
	}
}
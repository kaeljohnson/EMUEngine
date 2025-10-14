#include "../../include/Audio/AudioSystem.h"
#include "../../include/ECS/ECS.h"
#include "../../include/Components.h"
#include "../../include/AssetManager.h"
#include "../../include/ISDL/ISDL.h"

namespace Engine
{
	AudioSystem::AudioSystem(ECS& refECS, AssetManager& refAssetManager)
		: m_refECS(refECS), m_refAssetManager(refAssetManager)
	{
	}

	void AudioSystem::PlaySound(int soundIndex, int volume, const bool loop)
	{
		ENGINE_CRITICAL_D("Playing sound: " + std::to_string(soundIndex));
		Mix_Chunk* ptrSound = (Mix_Chunk*)m_refAssetManager.GetSound(soundIndex);
		if (!ptrSound)
		{
			ENGINE_CRITICAL("Sound not found: " + std::to_string(soundIndex));
			return;
		}

		Mix_VolumeChunk(ptrSound, volume);

		if (Mix_PlayChannel(-1, ptrSound, (loop ? -1 : 0)) == -1)
		{
			ENGINE_CRITICAL("No free channel to play sound!");
		}
	}
}
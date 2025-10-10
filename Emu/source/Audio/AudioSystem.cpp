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

	void AudioSystem::PlaySound(const std::string& soundName, int volume, const bool loop)
	{
		ENGINE_CRITICAL_D("Playing sound: " + soundName);
		Mix_Chunk* ptrSound = (Mix_Chunk*)m_refAssetManager.GetSound(soundName);
		Mix_VolumeChunk(ptrSound, volume);

		Mix_PlayChannel(-1, ptrSound, (loop ? -1 : 0));
	}
}
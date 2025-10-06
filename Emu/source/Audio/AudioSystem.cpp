#include "../../include/Audio/AudioSystem.h"

namespace Engine
{
	AudioInterface::AudioInterface(ECS& refECS)
		: m_refECS(refECS)
	{
	}

	AudioSystem::AudioSystem(ECS& refECS)
		: m_refECS(refECS)
	{
	}

}
#pragma once

#include "../ECS/ECS.h"

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
		AudioSystem(ECS& refECS);
		~AudioSystem() = default;



		// Deleted functions to ensure our AudioSystem instance cannot be copied or moved.
		AudioSystem(const AudioSystem&) = delete;
		AudioSystem& operator=(const AudioSystem&) = delete;
		AudioSystem(AudioSystem&&) = delete;
		AudioSystem& operator=(AudioSystem&&) = delete;

	private:

		ECS& m_refECS;
	};
}
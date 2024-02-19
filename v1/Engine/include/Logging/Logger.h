#pragma once

#include "spdlog/spdlog.h"

namespace Engine
{
	class Logger
	{
	private:
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& getEngineLogger() { return s_EngineLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_ClientLogger; }
	};

	#define ENGINE_TRACE(...)    ::Engine::Logger::getEngineLogger()->trace(__VA_ARGS__)
	#define ENGINE_INFO(...)     ::Engine::Logger::getEngineLogger()->info(__VA_ARGS__)
	#define ENGINE_WARN(...)     ::Engine::Logger::getEngineLogger()->warn(__VA_ARGS__)
	#define ENGINE_ERROR(...)    ::Engine::Logger::getEngineLogger()->error(__VA_ARGS__)
	#define ENGINE_CRITICAL(...) ::Engine::Logger::getEngineLogger()->critical(__VA_ARGS__)

	#define CLIENT_TRACE(...)    ::Engine::Logger::getClientLogger()->trace(__VA_ARGS__)
	#define CLIENT_INFO(...)     ::Engine::Logger::getClientLogger()->info(__VA_ARGS__)
	#define CLIENT_WARN(...)     ::Engine::Logger::getClientLogger()->warn(__VA_ARGS__)
	#define CLIENT_ERROR(...)    ::Engine::Logger::getClientLogger()->error(__VA_ARGS__)
	#define CLIENT_CRITICAL(...) ::Engine::Logger::getClientLogger()->critical(__VA_ARGS__)
}

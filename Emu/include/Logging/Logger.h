#pragma once

#include "../Core.h"
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

		inline static spdlog::logger& getEngineLogger() { return *s_EngineLogger; }
		EMU_API inline static spdlog::logger& getClientLogger() { return *s_ClientLogger; }
	};

#if defined(DEBUG)
	#define ENGINE_TRACE_D(...)	   ::Engine::Logger::getEngineLogger().trace(__VA_ARGS__)
	#define ENGINE_INFO_D(...)	   ::Engine::Logger::getEngineLogger().info(__VA_ARGS__)
	#define ENGINE_WARN_D(...)	   ::Engine::Logger::getEngineLogger().warn(__VA_ARGS__)
	#define ENGINE_ERROR_D(...)	   ::Engine::Logger::getEngineLogger().error(__VA_ARGS__)
	#define ENGINE_CRITICAL_D(...) ::Engine::Logger::getEngineLogger().critical(__VA_ARGS__)

	#define CLIENT_TRACE_D(...)	   ::Engine::Logger::getClientLogger().trace(__VA_ARGS__)
	#define CLIENT_INFO_D(...)	   ::Engine::Logger::getClientLogger().info(__VA_ARGS__)
	#define CLIENT_WARN_D(...)	   ::Engine::Logger::getClientLogger().warn(__VA_ARGS__)
	#define CLIENT_ERROR_D(...)	   ::Engine::Logger::getClientLogger().error(__VA_ARGS__)
	#define CLIENT_CRITICAL_D(...) ::Engine::Logger::getClientLogger().critical(__VA_ARGS__)
#elif defined(NDEBUG)
	#define ENGINE_TRACE_D(...)	 ((void)0)
	#define ENGINE_INFO_D(...)	 ((void)0)
	#define ENGINE_WARN_D(...)	 ((void)0)
	#define ENGINE_ERROR_D(...)	 ((void)0)
	#define ENGINE_CRITICAL_D(...) ((void)0)

	#define CLIENT_TRACE_D(...)	 ((void)0)
	#define CLIENT_INFO_D(...)	 ((void)0)
	#define CLIENT_WARN_D(...)	 ((void)0)
	#define CLIENT_ERROR_D(...)	 ((void)0)
	#define CLIENT_CRITICAL_D(...) ((void)0)
#endif

	#define ENGINE_TRACE(...)    ::Engine::Logger::getEngineLogger().trace(__VA_ARGS__)
	#define ENGINE_INFO(...)     ::Engine::Logger::getEngineLogger().info(__VA_ARGS__)
	#define ENGINE_WARN(...)     ::Engine::Logger::getEngineLogger().warn(__VA_ARGS__)
	#define ENGINE_ERROR(...)    ::Engine::Logger::getEngineLogger().error(__VA_ARGS__)
	#define ENGINE_CRITICAL(...) ::Engine::Logger::getEngineLogger().critical(__VA_ARGS__)

	#define CLIENT_TRACE(...)    ::Engine::Logger::getClientLogger().trace(__VA_ARGS__)
	#define CLIENT_INFO(...)     ::Engine::Logger::getClientLogger().info(__VA_ARGS__)
	#define CLIENT_WARN(...)     ::Engine::Logger::getClientLogger().warn(__VA_ARGS__)
	#define CLIENT_ERROR(...)    ::Engine::Logger::getClientLogger().error(__VA_ARGS__)
	#define CLIENT_CRITICAL(...) ::Engine::Logger::getClientLogger().critical(__VA_ARGS__)
}

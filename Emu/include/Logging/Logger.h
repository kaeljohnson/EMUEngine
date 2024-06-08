#pragma once

#include "../Core.h"

#include <memory>

namespace Engine
{
	class Logger
	{
	public:
		Logger();

		template <typename... Args>
		void EngineTrace(Args... message);

		template <typename... Args>
		void EngineInfo(Args... message);

		template <typename... Args>
		void EngineWarn(Args... message);

		template <typename... Args>
		void EngineError(Args... message);

		template <typename... Args>
		void EngineCritical(Args... message);

		template <typename... Args>
		void ClientTrace(Args... message);
		
		template <typename... Args>
		void ClientInfo(Args... message);
		
		template <typename... Args>
		void ClientWarn(Args... message);
		
		template <typename... Args>
		void ClientError(Args... message);
		
		template <typename... Args>
		void ClientCritical(Args... message);

	private:
		
	};

	EMU_API extern std::shared_ptr<Logger> GlobalLogger;

	std::shared_ptr<Logger> CreateLogger();
}

#if defined(DEBUG)
#define ENGINE_TRACE_D(msg)    ::Engine::GlobalLogger->EngineTrace(msg)
#define ENGINE_INFO_D(msg)     ::Engine::GlobalLogger->EngineInfo(msg)
#define ENGINE_WARN_D(msg)     ::Engine::GlobalLogger->EngineWarn(msg)
#define ENGINE_ERROR_D(msg)    ::Engine::GlobalLogger->EngineError(msg)
#define ENGINE_CRITICAL_D(msg) ::Engine::GlobalLogger->EngineCritical(msg)

#define CLIENT_TRACE_D(msg)    ::Engine::GlobalLogger->ClientTrace(msg)
#define CLIENT_INFO_D(msg)     ::Engine::GlobalLogger->ClientInfo(msg)
#define CLIENT_WARN_D(msg)     ::Engine::GlobalLogger->ClientWarn(msg)
#define CLIENT_ERROR_D(msg)    ::Engine::GlobalLogger->ClientError(msg)
#define CLIENT_CRITICAL_D(msg) ::Engine::GlobalLogger->ClientCritical(msg)
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

#define ENGINE_TRACE(msg)    ::Engine::GlobalLogger->EngineTrace(msg)
#define ENGINE_INFO(msg)     ::Engine::GlobalLogger->EngineInfo(msg)
#define ENGINE_WARN(msg)     ::Engine::GlobalLogger->EngineWarn(msg)
#define ENGINE_ERROR(msg)    ::Engine::GlobalLogger->EngineError(msg)
#define ENGINE_CRITICAL(msg) ::Engine::GlobalLogger->EngineCritical(msg)

#define CLIENT_TRACE(msg)    ::Engine::GlobalLogger->ClientTrace(msg)
#define CLIENT_INFO(msg)     ::Engine::GlobalLogger->ClientInfo(msg)
#define CLIENT_WARN(msg)     ::Engine::GlobalLogger->ClientWarn(msg)
#define CLIENT_ERROR(msg)    ::Engine::GlobalLogger->ClientError(msg)
#define CLIENT_CRITICAL(msg) ::Engine::GlobalLogger->ClientCritical(msg)
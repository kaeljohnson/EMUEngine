#pragma once

#include "../Core.h"

#include <string>
#include <memory>

namespace Engine
{
	class Logger
	{
	public:
		Logger();

		EMU_API void EngineTrace(const std::string& message);
		EMU_API void EngineInfo(const std::string& message);
		EMU_API void EngineWarn(const std::string& message);
		EMU_API void EngineError(const std::string& message);
		EMU_API void EngineCritical(const std::string& message);
		EMU_API void ClientTrace(const std::string& message);
		EMU_API void ClientInfo(const std::string& message);
		EMU_API void ClientWarn(const std::string& message);
		EMU_API void ClientError(const std::string& message);
		EMU_API void ClientCritical(const std::string& message);

	private:
		
	};

	EMU_API extern std::shared_ptr<Logger> GlobalLogger;
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
#pragma once

#include "../Core.h"
#include "../Includes.h"

namespace Engine
{
	class Logger
	{
	public:
		Logger();

		template<typename... Args>
		void EngineTrace(std::string_view fmt, Args&&... args)
		{
			engineTrace(std::vformat(fmt, std::make_format_args(args...)));
		}

		template<typename... Args>
		void EngineInfo(std::string_view fmt, Args&&... args)
		{
			engineInfo(std::vformat(fmt, std::make_format_args(args...)));
		}

		template<typename... Args>
		void EngineWarn(std::string_view fmt, Args&&... args)
		{
			engineWarn(std::vformat(fmt, std::make_format_args(args...)));
		}

		template<typename... Args>
		void EngineError(std::string_view fmt, Args&&... args)
		{
			engineError(std::vformat(fmt, std::make_format_args(args...)));
		}

		template<typename... Args>
		void EngineCritical(std::string_view fmt, Args&&... args)
		{
			engineCritical(std::vformat(fmt, std::make_format_args(args...)));
		}

		template<typename... Args>
		void ClientTrace(std::string_view fmt, Args&&... args)
		{
			clientTrace(std::vformat(fmt, std::make_format_args(args...)));
		}

		template<typename... Args>
		void ClientInfo(std::string_view fmt, Args&&... args)
		{
			clientInfo(std::vformat(fmt, std::make_format_args(args...)));
		}

		template<typename... Args>
		void ClientWarn(std::string_view fmt, Args&&... args)
		{
			clientWarn(std::vformat(fmt, std::make_format_args(args...)));
		}

		template<typename... Args>
		void ClientError(std::string_view fmt, Args&&... args)
		{
			clientError(std::vformat(fmt, std::make_format_args(args...)));
		}

		template<typename... Args>
		void ClientCritical(std::string_view fmt, Args&&... args)
		{
			clientCritical(std::vformat(fmt, std::make_format_args(args...)));
		}

	private:
		EMU_API void engineTrace(const std::string& message);
		EMU_API void engineInfo(const std::string& message);
		EMU_API void engineWarn(const std::string& message);
		EMU_API void engineError(const std::string& message);
		EMU_API void engineCritical(const std::string& message);
		EMU_API void clientTrace(const std::string& message);
		EMU_API void clientInfo(const std::string& message);
		EMU_API void clientWarn(const std::string& message);
		EMU_API void clientError(const std::string& message);
		EMU_API void clientCritical(const std::string& message);
	};

	EMU_API extern std::shared_ptr<Logger> GlobalLogger;
}

#if defined(DEBUG)
#define ENGINE_TRACE_D(...)    ::Engine::GlobalLogger->EngineTrace(__VA_ARGS__)
#define ENGINE_INFO_D(...)     ::Engine::GlobalLogger->EngineInfo(__VA_ARGS__)
#define ENGINE_WARN_D(...)     ::Engine::GlobalLogger->EngineWarn(__VA_ARGS__)
#define ENGINE_ERROR_D(...)    ::Engine::GlobalLogger->EngineError(__VA_ARGS__)
#define ENGINE_CRITICAL_D(...) ::Engine::GlobalLogger->EngineCritical(__VA_ARGS__)

#define CLIENT_TRACE_D(...)    ::Engine::GlobalLogger->ClientTrace(__VA_ARGS__)
#define CLIENT_INFO_D(...)     ::Engine::GlobalLogger->ClientInfo(__VA_ARGS__)
#define CLIENT_WARN_D(...)     ::Engine::GlobalLogger->ClientWarn(__VA_ARGS__)
#define CLIENT_ERROR_D(...)    ::Engine::GlobalLogger->ClientError(__VA_ARGS__)
#define CLIENT_CRITICAL_D(...) ::Engine::GlobalLogger->ClientCritical(__VA_ARGS__)
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

#define ENGINE_TRACE(...) ::Engine::GlobalLogger->EngineTrace(__VA_ARGS__)
#define ENGINE_INFO(...)  ::Engine::GlobalLogger->EngineInfo(__VA_ARGS__)
#define ENGINE_WARN(...)  ::Engine::GlobalLogger->EngineWarn(__VA_ARGS__)
#define ENGINE_ERROR(...) ::Engine::GlobalLogger->EngineError(__VA_ARGS__)
#define ENGINE_CRITICAL(...) ::Engine::GlobalLogger->EngineCritical(__VA_ARGS__)

#define CLIENT_TRACE(...) ::Engine::GlobalLogger->ClientTrace(__VA_ARGS__)
#define CLIENT_INFO(...)  ::Engine::GlobalLogger->ClientInfo(__VA_ARGS__)
#define CLIENT_WARN(...)  ::Engine::GlobalLogger->ClientWarn(__VA_ARGS__)
#define CLIENT_ERROR(...) ::Engine::GlobalLogger->ClientError(__VA_ARGS__)
#define CLIENT_CRITICAL(...) ::Engine::GlobalLogger->ClientCritical(__VA_ARGS__)

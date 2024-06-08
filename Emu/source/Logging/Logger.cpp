#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{

	std::shared_ptr<spdlog::logger> s_EngineLogger;
	std::shared_ptr<spdlog::logger> s_ClientLogger;

	std::shared_ptr<Logger> GlobalLogger = CreateLogger();

	Logger::Logger()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_EngineLogger = spdlog::stdout_color_mt("ENGINE");
		s_EngineLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	template <typename... Args>
	void Logger::EngineTrace(Args... message)
	{
		s_EngineLogger->trace(message);
	}
	
	template <typename... Args>
	void Logger::EngineInfo(Args... message)
	{
		s_EngineLogger->info(message);
	}
	
	template <typename... Args>
	void Logger::EngineWarn(Args... message)
	{
		s_EngineLogger->warn(message);
	}
	
	template <typename... Args>
	void Logger::EngineError(Args... message)
	{
		s_EngineLogger->error(message);
	}
	
	template <typename... Args>
	void Logger::EngineCritical(Args... message)
	{
		s_EngineLogger->critical(message);
	}

	template <typename... Args>
	void Logger::ClientTrace(Args... message)
	{
		s_ClientLogger->trace(message);
	}

	template <typename... Args>
	void Logger::ClientInfo(Args... message)
	{
		s_ClientLogger->info(message);
	}

	template <typename... Args>
	void Logger::ClientWarn(Args... message)
	{
		s_ClientLogger->warn(message);
	}

	template <typename... Args>
	void Logger::ClientError(Args... message)
	{
		s_ClientLogger->error(message);
	}

	template <typename... Args>
	void Logger::ClientCritical(Args... message)
	{
		s_ClientLogger->critical(message);
	}
}
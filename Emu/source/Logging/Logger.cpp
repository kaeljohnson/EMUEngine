#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	std::shared_ptr<Logger> GlobalLogger = std::make_shared<Logger>();

	std::shared_ptr<spdlog::logger> s_EngineLogger;
	std::shared_ptr<spdlog::logger> s_ClientLogger;

	Logger::Logger()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_EngineLogger = spdlog::stdout_color_mt("ENGINE");
		s_EngineLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	void Logger::EngineTrace(const std::string& message)
	{
		s_EngineLogger->trace(message);
	}
	
	void Logger::EngineInfo(const std::string& message)
	{
		s_EngineLogger->info(message);
	}
	
	void Logger::EngineWarn(const std::string& message)
	{
		s_EngineLogger->warn(message);
	}
	
	void Logger::EngineError(const std::string& message)
	{
		s_EngineLogger->error(message);
	}
	
	void Logger::EngineCritical(const std::string& message)
	{
		s_EngineLogger->critical(message);
	}

	void Logger::ClientTrace(const std::string& message)
	{
		s_ClientLogger->trace(message);
	}

	void Logger::ClientInfo(const std::string& message)
	{
		s_ClientLogger->info(message);
	}

	void Logger::ClientWarn(const std::string& message)
	{
		s_ClientLogger->warn(message);
	}

	void Logger::ClientError(const std::string& message)
	{
		s_ClientLogger->error(message);
	}

	void Logger::ClientCritical(const std::string& message)
	{
		s_ClientLogger->critical(message);
	}
}
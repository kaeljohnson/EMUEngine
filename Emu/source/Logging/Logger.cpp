#pragma once

#include "../../include/Logging/Logger.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

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

	void Logger::engineTrace(const std::string& message)
	{
		s_EngineLogger->trace(message);
	}
	
	void Logger::engineInfo(const std::string& message)
	{
		s_EngineLogger->info(message);
	}
	
	void Logger::engineWarn(const std::string& message)
	{
		s_EngineLogger->warn(message);
	}
	
	void Logger::engineError(const std::string& message)
	{
		s_EngineLogger->error(message);
	}
	
	void Logger::engineCritical(const std::string& message)
	{
		s_EngineLogger->critical(message);
	}

	void Logger::clientTrace(const std::string& message)
	{
		s_ClientLogger->trace(message);
	}

	void Logger::clientInfo(const std::string& message)
	{
		s_ClientLogger->info(message);
	}

	void Logger::clientWarn(const std::string& message)
	{
		s_ClientLogger->warn(message);
	}

	void Logger::clientError(const std::string& message)
	{
		s_ClientLogger->error(message);
	}

	void Logger::clientCritical(const std::string& message)
	{
		s_ClientLogger->critical(message);
	}
}
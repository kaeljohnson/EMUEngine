#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
	// std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;
	// std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;
	
	std::shared_ptr<ILogger> GlobalLogger = CreateLogger();

	Logger::Logger()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_EngineLogger = spdlog::stdout_color_mt("ENGINE");
		s_EngineLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	void Logger::Trace(const std::string& message)
	{
		s_EngineLogger->trace(message);
	}
	
	void Logger::Info(const std::string& message)
	{
		s_EngineLogger->info(message);
	}
	
	void Logger::Warn(const std::string& message)
	{
		s_EngineLogger->warn(message);
	}
	
	void Logger::Error(const std::string& message)
	{
		s_EngineLogger->error(message);
	}
	
	void Logger::Critical(const std::string& message)
	{
		s_EngineLogger->critical(message);
	}
}
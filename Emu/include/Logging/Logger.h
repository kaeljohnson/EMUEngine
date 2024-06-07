
#pragma once

#include "ILogger.h"
#include "spdlog/spdlog.h"
#include <memory>

namespace Engine
{
	class Logger : public ILogger
	{
	public:
		Logger();

		void EngineTrace(const std::string& message) override;
		void EngineInfo(const std::string& message) override;
		void EngineWarn(const std::string& message) override;
		void EngineError(const std::string& message) override;
		void EngineCritical(const std::string& message) override;

		void ClientTrace(const std::string& message) override;
		void ClientInfo(const std::string& message) override;
		void ClientWarn(const std::string& message) override;
		void ClientError(const std::string& message) override;
		void ClientCritical(const std::string& message) override;

	private:
		std::shared_ptr<spdlog::logger> s_EngineLogger;
		std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}
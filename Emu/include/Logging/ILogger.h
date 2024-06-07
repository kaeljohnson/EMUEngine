#pragma once

#include "../Core.h"

#include <string>
#include <memory>

namespace Engine
{
    class ILogger
    {
    public:
        virtual ~ILogger() = default;

        virtual void Trace(const std::string& message) = 0;
        virtual void Info(const std::string& message) = 0;
        virtual void Warn(const std::string& message) = 0;
        virtual void Error(const std::string& message) = 0;
        virtual void Critical(const std::string& message) = 0;
    };

    EMU_API extern std::shared_ptr<ILogger> GlobalLogger;

    std::shared_ptr<ILogger> CreateLogger();
}

#if defined(DEBUG)
#define ENGINE_TRACE_D(msg)    ::Engine::GlobalLogger->Trace(msg)
#define ENGINE_INFO_D(msg)     ::Engine::GlobalLogger->Info(msg)
#define ENGINE_WARN_D(msg)     ::Engine::GlobalLogger->Warn(msg)
#define ENGINE_ERROR_D(msg)    ::Engine::GlobalLogger->Error(msg)
#define ENGINE_CRITICAL_D(msg) ::Engine::GlobalLogger->Critical(msg)

//#define CLIENT_TRACE_D(msg)    ::Engine::GlobalLogger->Trace(msg)
//#define CLIENT_INFO_D(msg)     ::Engine::GlobalLogger->Info(msg)
//#define CLIENT_WARN_D(msg)     ::Engine::GlobalLogger->Warn(msg)
//#define CLIENT_ERROR_D(msg)    ::Engine::GlobalLogger->Error(msg)
//#define CLIENT_CRITICAL_D(msg) ::Engine::GlobalLogger->Critical(msg)
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

#define ENGINE_TRACE(msg)    ::Engine::GlobalLogger->Trace(msg)
#define ENGINE_INFO(msg)     ::Engine::GlobalLogger->Info(msg)
#define ENGINE_WARN(msg)     ::Engine::GlobalLogger->Warn(msg)
#define ENGINE_ERROR(msg)    ::Engine::GlobalLogger->Error(msg)
#define ENGINE_CRITICAL(msg) ::Engine::GlobalLogger->Critical(msg)

//#define CLIENT_TRACE(msg)    ::Engine::GlobalClientLogger->Trace(msg)
//#define CLIENT_INFO(msg)     ::Engine::GlobalClientLogger->Info(msg)
//#define CLIENT_WARN(msg)     ::Engine::GlobalClientLogger->Warn(msg)
//#define CLIENT_ERROR(msg)    ::Engine::GlobalClientLogger->Error(msg)
//#define CLIENT_CRITICAL(msg) ::Engine::GlobalClientLogger->Critical(msg)
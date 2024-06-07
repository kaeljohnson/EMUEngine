#pragma once

#include <memory>


#include "../../include/Logging/ILogger.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{
    std::shared_ptr<ILogger> CreateLogger()
    {
        return std::make_shared<Logger>();
    }
}
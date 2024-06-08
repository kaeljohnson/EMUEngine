#pragma once

#include <memory>


#include "../../include/Logging/Logger.h"

namespace Engine
{
    std::shared_ptr<Logger> CreateLogger()
    {
        return std::make_shared<Logger>();
    }
}
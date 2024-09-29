#pragma once

#include "../Core.h"

#include <functional>
#include <memory>

#include "ComponentManager.h"

namespace Engine
{ 
    class Updatable 
    {
    public:
        using UpdateCallback = std::function<void()>;

        EMU_API Updatable(size_t id, UpdateCallback callback);

        void Update();

        EMU_API ~Updatable();

        EMU_API size_t GetID() const;

    public:
        size_t m_id;
        UpdateCallback m_callback;
    };
}
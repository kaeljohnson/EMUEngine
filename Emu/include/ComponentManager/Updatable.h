#pragma once

#include "../Component.h"

#include "../Core.h"

#include <functional>

#include "ComponentManager.h"

namespace Engine
{ 
	class Updatable : public Component
    {
    public:
        using UpdateCallback = std::function<void()>;

        EMU_API Updatable(size_t id, UpdateCallback callback);

        EMU_API ~Updatable();

    public:
        // const size_t m_id;
        const UpdateCallback m_callback;

        void Update();
    };
}
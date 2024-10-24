#pragma once

#include "../ECS/Component.h"

#include "../Core.h"

#include <functional>

#include "../ECS/ComponentManager.h"

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
        UpdateCallback m_callback;

        void Update();
    };
}
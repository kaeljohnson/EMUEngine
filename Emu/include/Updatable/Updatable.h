#pragma once

#include "../ECS/Component.h"
#include "../ECS/Entity.h"

#include "../Core.h"

#include <functional>

namespace Engine
{ 
	class Updatable : public Component
    {
    public:
        using UpdateCallback = std::function<void()>;

        EMU_API Updatable(Entity& entity, UpdateCallback callback);

        EMU_API ~Updatable();

    public:
        // const size_t m_id;
        UpdateCallback m_callback;

        void Update();
    };
}
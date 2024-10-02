#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>

#include "../Logging/Logger.h"

namespace Engine
{
    class ComponentManagerBase 
    {
    public:
        virtual ~ComponentManagerBase() = default;
    };

    template <typename T>
    class ComponentManager : public ComponentManagerBase
    {
    public:
        void AddComponent(size_t objectID, T* component) 
        {
            // ENGINE_INFO_D("Adding component: " + std::to_string(objectID));
            m_components[objectID] = component;
        }

        void RemoveComponent(size_t objectID)
        {
            m_components.erase(objectID);
        }

        void AddToActiveComponents(size_t objectID)
        {
            // Maybe instead of storing pointers, store actual objects and just std::move them?
            auto it = m_components.find(objectID);
            if (it != m_components.end())
            {
                m_activeComponents.push_back(it->second);
            }
        }

        void RemoveFromActiveComponents(size_t objectID)
        {
            m_activeComponents.erase(
                std::remove_if(m_activeComponents.begin(), m_activeComponents.end(),
                    [objectID](T* component) {
                        return component && component->m_id == objectID;
                    }),
                m_activeComponents.end()
            );
        }

        T& GetComponent(size_t entity)
        {
            auto it = m_components.find(entity);
            if (it != m_components.end())
            {
                return it->second;
            }
            return nullptr;
        }

        std::unordered_map<size_t, T>& GetAllComponents() 
        {
            return m_components;
        }

		std::vector<T>& GetActiveComponents()
		{
			return m_activeComponents;
		}

    private:
		// Key: Entity ID, Component index
        std::unordered_map<size_t, size_t> m_components;
        std::vector<T> m_activeComponents;
    };

    class ComponentManagerRegistry 
    {
    public:
        template <typename T>
        static ComponentManager<T>& GetManager()
        {
            static ComponentManager<T> manager;
            return manager;
        }
    };
}
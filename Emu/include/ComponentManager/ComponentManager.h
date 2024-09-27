#pragma once

#include <unordered_map>

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
            m_components[objectID] = component;
        }

        void RemoveComponent(size_t objectID)
        {
            m_components.erase(objectID);
        }

        T* GetComponent(size_t entity)
        {
            auto it = m_components.find(entity);
            if (it != m_components.end())
            {
                return it->second;
            }
            return nullptr;
        }

        std::unordered_map<size_t, T*>& GetAllComponents() 
        {
            return m_components;
        }

    private:
        std::unordered_map<size_t, T*> m_components;
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
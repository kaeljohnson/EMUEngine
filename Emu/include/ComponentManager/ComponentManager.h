#pragma once

#include <unordered_map>
#include <memory>
#include <string>

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
        void AddComponent(std::string& objectID, T component) 
        {
            m_components[objectID] = component;
        }

        T* GetComponent(std::string& entity)
        {
            auto it = m_components.find(entity);
            if (it != m_components.end()) 
            {
                return &it->second;
            }
            return nullptr;
        }

        std::unordered_map<std::string&, T>& GetAllComponents() 
        {
            return m_components;
        }

    private:
        std::unordered_map<std::string&, T> m_components;
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
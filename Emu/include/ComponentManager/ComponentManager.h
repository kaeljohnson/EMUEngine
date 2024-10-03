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
        template <typename T>
        void AddComponent(size_t id, const T& component) 
        {
            if (m_idToIndex.find(id) != m_idToIndex.end()) 
            {
                // Component already exists for this entity, update it instead
                components[m_idToIndex[id]] = component;
            }
            else 
            {
                // Use emplace_back to construct the component in place, no copying
                components.emplace_back(component);

                // Store the index of the new component
                size_t newIndex = components.size() - 1;
                m_idToIndex[id] = newIndex;
                // indexToEntity[newIndex] = entity;
            }
        }

        template<typename... Args>
        void AddComponent(size_t id, Args&&... args) 
        {
            if (m_idToIndex.find(id) != m_idToIndex.end()) 
            {
                // Update the existing component by constructing it with new arguments
                m_components[m_idToIndex[id]] = ComponentType(std::forward<Args>(args)...);
            }
            else 
            { 
                // Use emplace_back to construct the component in place from arguments
                m_components.emplace_back(std::forward<Args>(args)...);

                // Store the index of the new component
                size_t newIndex = components.size() - 1;
                m_idToIndex[id] = newIndex;
                // indexToEntity[newIndex] = entity;
            }
        }

        template<typename... Args>
        T* AddAndGetComponent(size_t id, Args&&... args)
        {
            if (m_idToIndex.find(id) != m_idToIndex.end())
            {
                // Update the existing component by constructing it with new arguments
                m_components[m_idToIndex[id]] = ComponentType(std::forward<Args>(args)...);
            }
            else
            {
                // Use emplace_back to construct the component in place from arguments
                m_components.emplace_back(std::forward<Args>(args)...);

                // Store the index of the new component
                size_t newIndex = components.size() - 1;
                m_idToIndex[id] = newIndex;
                // indexToEntity[newIndex] = entity;
            }

            return m_components[m_idToIndex[id]]
        }

        template<typename T>
        void AddAndGetComponent(size_t id)
        {
            m_components.emplace_back();
            entityToIndex[id] = components.size() - 1;  // Map entity to index
        }
        
        T* GetComponent(size_t id) 
        {
            return &m_components[m_idToIndex[id]];
        }

        bool HasComponent(size_t id) const 
        {
            return m_idToIndex.find(id) != m_idToIndex.end();
        }

        

        /*void RemoveComponent(size_t objectID)
        {
            m_components.erase(objectID);
        }*/

        /*T& GetComponent(size_t entity)
        {
            auto it = m_idToIndex.find(entity);
            if (it != m_idToIndex.end())
            {
                return it->second;
            }
            return nullptr;
        }*/

        /*std::unordered_map<size_t, T>& GetAllComponents() 
        {
            return m_components;
        }*/

		/*std::vector<T>& GetActiveComponents()
		{
			return m_activeComponents;
		}*/

    private:
        // Component ID, Index
        std::unordered_map<size_t, size_t> m_idToIndex;
        std::vector<T> m_components;
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
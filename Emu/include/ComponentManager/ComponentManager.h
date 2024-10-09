#pragma once

#include <iostream>

#include <vector>
#include <unordered_map>
#include <algorithm>

#include "../Core.h"

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
				// Replace the existing component with the new one
                m_components[m_idToIndex[id]] = std::move(component);
            }
            else 
            {
                // Use emplace_back to construct the component in place, no copying
                m_components.emplace_back(component);

                // Store the index of the new component
                size_t newIndex = m_components.size() - 1;
                m_idToIndex[id] = newIndex;
                // indexToEntity[newIndex] = entity;
            }
        }

        template<typename... Args>
        void AddComponent(Args&&... args) 
        {
            auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
            auto& id = std::get<0>(argsTuple);
            ENGINE_CRITICAL_D("ADDING COMPONENT: " + std::to_string(id));
            if (m_idToIndex.find(id) != m_idToIndex.end()) 
            {
                ENGINE_CRITICAL_D("Replacing component with ID: " + std::to_string(id));
                // Update the existing component by constructing it with new arguments
                size_t index = m_idToIndex[id];
                m_components[index].~T();
                new (&m_components[index]) T(std::forward<Args>(args)...);
            }
            else 
            { 
                // Use emplace_back to construct the component in place from arguments
                m_components.emplace_back(std::forward<Args>(args)...);
                // Store the index of the new component
                size_t newIndex = m_components.size() - 1;
                m_idToIndex[id] = newIndex;
                // indexToEntity[newIndex] = entity;
            }
        }

        template<typename... Args>
        T* AddAndGetComponent(Args&&... args)
        {
            auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
            auto& id = std::get<0>(argsTuple);

			AddComponent(std::forward<Args>(args)...);

            return &m_components[m_idToIndex[id]];
        }
        
		void ActivateComponent(size_t id)
		{
			if (m_idToIndex.find(id) != m_idToIndex.end())
			{
				m_components[m_idToIndex[id]].SetActive(true);
			}
		}

        void DeactivateComponent(size_t id)
        {
            if (m_idToIndex.find(id) != m_idToIndex.end())
            {
                m_components[m_idToIndex[id]].SetActive(false);
            }
        }

        T* GetComponent(size_t id) 
        {
			if (m_idToIndex.find(id) == m_idToIndex.end())
			{
				return nullptr;
			}
            return &m_components[m_idToIndex[id]];
        }

        bool HasComponent(size_t id) const 
        {
            return m_idToIndex.find(id) != m_idToIndex.end();
        }

		std::vector<T>& GetComponents()
		{
			return m_components;
		}

		void Allocate(size_t size)
		{
			m_components.reserve(size);
		}

    private:
        // Component ID, Index
        std::unordered_map<size_t, size_t> m_idToIndex;
        std::vector<T> m_components;
    };

    class ComponentManagerRegistry 
    {
    public:
        template <typename T>
        EMU_API static ComponentManager<T>& GetManager();
    };
}
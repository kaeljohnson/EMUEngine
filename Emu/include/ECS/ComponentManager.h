#pragma once

#include <iostream>

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <utility>

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
        using iterator = typename std::vector<T>::iterator;
        using const_iterator = typename std::vector<T>::const_iterator;

        // Iterator for all components (begin to end)
        auto begin() { return m_components.begin(); }
        auto end() { return m_components.end(); }
        auto begin() const { return m_components.begin(); }
        auto end() const { return m_components.end(); }

        // Iterator for active components (from begin to m_activeComponentCount)
        auto active_begin() { return m_components.begin(); }
        auto active_end() { return m_components.begin() + m_activeComponentCount; }
        auto active_begin() const { return m_components.begin(); }
        auto active_end() const { return m_components.begin() + m_activeComponentCount; }


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

		void ActivateComponents(std::vector<size_t>& ids)
		{
            for (auto& id : ids)
            {
                if (m_idToIndex.find(id) == m_idToIndex.end()) { continue; }

                size_t currentIndex = m_idToIndex[id];

                // Swap components directly without creating a temporary object
                std::swap(m_components[currentIndex], m_components[m_activeComponentCount]);

                // Swap indices
                m_idToIndex[m_components[m_activeComponentCount].GetID()] = currentIndex;
                m_idToIndex[id] = m_activeComponentCount;

                m_components[m_activeComponentCount].SetActive(true);

                // Increment the count of active components
                ++m_activeComponentCount;
            }

		}

        void DeactivateComponents(std::vector<size_t>& ids)
        {
            for (auto& id : ids)
            {
                if (m_idToIndex.find(id) != m_idToIndex.end())
                {
                    m_components[m_idToIndex[id]].SetActive(false);
                    m_activeComponentCount--;
                }
            }
        }

        void RemoveComponent(size_t id)
        {
            if (m_idToIndex.find(id) == m_idToIndex.end()) 
            {
                return; // Component doesn't exist
            }

            size_t indexToRemove = m_idToIndex[id];

			if (!m_components[indexToRemove].IsActive())
			{
				std::swap(m_components[indexToRemove], m_components[m_components.size() - 1]);
				m_idToIndex[m_components[indexToRemove].GetID()] = indexToRemove;
                return;
			}

            // Component Active Case
			// shift all active components to the left
			// Once all active components are shifted, decrement the active component count
			// swap the now empty slot with the last component
			// remove the last component
			for (size_t i = indexToRemove; i < m_activeComponentCount - 1; ++i)
			{
				std::swap(m_components[i], m_components[i + 1]);
				m_idToIndex[m_components[i].GetID()] = i;
			}

			m_idToIndex.erase(id);
			m_components.pop_back();
			--m_activeComponentCount;
        }



        T* GetComponent(size_t id) 
        {
			// Call HasComponent first to check if the component exists
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

		const size_t GetActiveComponentCount() const
		{
			return m_activeComponentCount;
		}

		void Allocate(size_t size)
		{
			m_components.reserve(size);
		}

    private:
        // Component ID, Index
        std::unordered_map<size_t, size_t> m_idToIndex;
        std::vector<T> m_components;

		size_t m_activeComponentCount = 0;
    };

    class ComponentManagerRegistry 
    {
    public:
        template <typename T>
        EMU_API static ComponentManager<T>& GetManager();
    };
}
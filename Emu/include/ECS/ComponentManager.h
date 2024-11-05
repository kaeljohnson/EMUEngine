#pragma once

#include <vector>
#include <unordered_map>

namespace Engine
{
    class ComponentManagerBase 
    {
    public:
        virtual ~ComponentManagerBase() = default;
        virtual void Allocate(size_t size) = 0;
        virtual void RemoveComponent(size_t id) = 0;
		virtual void ActivateComponents(std::vector<size_t>& ids) = 0;
		virtual void DeactivateComponents() = 0;
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

        // Iterator for active components (from begin to end)
        auto active_begin() { return m_activeComponents.begin(); }
        auto active_end() { return m_activeComponents.end(); }
        auto active_begin() const { return m_activeComponents.begin(); }
        auto active_end() const { return m_activeComponents.end(); }

        template<typename... Args>
        void AddComponent(Args&&... args)
        {
            auto argsTuple = std::make_tuple(std::forward<Args>(args)...);
            auto& id = std::get<0>(argsTuple);

            if (m_inactiveIdToIndex.find(id) != m_inactiveIdToIndex.end() ||
                m_activeIdToIndex.find(id) != m_activeIdToIndex.end())
            {
                return; // Component already exists
            }

            // Add a new inactive component to m_components
            m_components.emplace_back(std::forward<Args>(args)...);
            m_inactiveIdToIndex[id] = m_components.size() - 1;
        }

        void ActivateComponent(size_t id)
        {
            auto it = m_inactiveIdToIndex.find(id);
            if (it == m_inactiveIdToIndex.end()) return; // Component is not inactive

            size_t index = it->second;

            // Move the component to m_activeComponents
            m_activeComponents.push_back(std::move(m_components[index]));
            m_activeIdToIndex[id] = m_activeComponents.size() - 1;

            // Remove the component from m_components by swapping and popping
            if (index != m_components.size() - 1)
            {
                std::swap(m_components[index], m_components.back());
                m_inactiveIdToIndex[m_components[index].GetID()] = index;
            }
            m_components.pop_back();
            m_inactiveIdToIndex.erase(it);
        }

		void ActivateComponents(std::vector<size_t>& ids) override
		{
			for (size_t id : ids)
			{
				ActivateComponent(id);
			}
		}

        void DeactivateComponent(size_t id)
        {
            auto it = m_activeIdToIndex.find(id);
            if (it == m_activeIdToIndex.end()) return; // Component is not active

            size_t index = it->second;

            // Move the component back to m_components
            m_components.push_back(std::move(m_activeComponents[index]));
            m_inactiveIdToIndex[id] = m_components.size() - 1;

            // Remove the component from m_activeComponents by swapping and popping
            if (index != m_activeComponents.size() - 1)
            {
                std::swap(m_activeComponents[index], m_activeComponents.back());
                m_activeIdToIndex[m_activeComponents[index].GetID()] = index;
            }
            m_activeComponents.pop_back();
            m_activeIdToIndex.erase(it);
        }

		void DeactivateComponents()
		{
            std::vector<size_t> keysToDeactivate;
            for (const auto& pair : m_activeIdToIndex)
            {
                keysToDeactivate.push_back(pair.first);
            }

            for (const auto& key : keysToDeactivate)
            {
                DeactivateComponent(key);
            }
		}

        void RemoveComponent(size_t id) override
        {
            if (m_inactiveIdToIndex.count(id))
            {
                // Remove from inactive components
                size_t index = m_inactiveIdToIndex[id];
                if (index != m_components.size() - 1) {
                    std::swap(m_components[index], m_components.back());
                    m_inactiveIdToIndex[m_components[index].GetID()] = index;
                }
                m_components.pop_back();
                m_inactiveIdToIndex.erase(id);
            }
            else if (m_activeIdToIndex.count(id))
            {
                // Remove from active components
                size_t index = m_activeIdToIndex[id];
                if (index != m_activeComponents.size() - 1) {
                    std::swap(m_activeComponents[index], m_activeComponents.back());
                    m_activeIdToIndex[m_activeComponents[index].GetID()] = index;
                }
                m_activeComponents.pop_back();
                m_activeIdToIndex.erase(id);
            }
        }

        T* GetComponent(size_t id)
        {

            if (m_activeIdToIndex.count(id))
                return &m_activeComponents[m_activeIdToIndex[id]];

            if (m_inactiveIdToIndex.count(id))
                return &m_components[m_inactiveIdToIndex[id]];

            return nullptr; // Component doesn't exist
        }

        bool HasComponent(size_t id) const
        {
            return m_inactiveIdToIndex.count(id) || m_activeIdToIndex.count(id);
        }

        std::vector<T>& GetComponents() { return m_components; }

        void Allocate(size_t size) override
        {
            m_components.reserve(size);
            m_activeComponents.reserve(size);
        }

    private:
        // Separate ID-to-index maps for active and inactive components
        std::unordered_map<size_t, size_t> m_inactiveIdToIndex;
        std::unordered_map<size_t, size_t> m_activeIdToIndex;

        std::vector<T> m_components;          // Holds inactive components
        std::vector<T> m_activeComponents;    // Holds active components
    };

}
#pragma once

class Component
{
public:
	Component(const size_t id) : m_id(id) {}
	virtual ~Component() = default;

	inline const size_t GetID() const { return m_id; }
	inline const bool IsActive() const { return m_isActive; }
	inline const bool IsLastActive() const { return m_isLastActive; }
	inline const size_t GetLayerIdx() const { return m_layerIdx; }

	inline void SetActive(const bool active) { m_isActive = active; }

protected:
	size_t m_id;
	bool m_isActive = false;

	bool m_isLastActive = false;
	size_t m_layerIdx = 0;
};
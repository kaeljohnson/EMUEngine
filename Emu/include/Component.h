#pragma once

class Component
{
public:
	Component(const size_t id) : m_id(id) {}
	virtual ~Component() = default;

	inline const size_t GetID() const { return m_id; }
	inline const bool IsActive() const { return m_isActive; }

	inline void SetActive(const bool active) { m_isActive = active; }

protected:
	size_t m_id;
	bool m_isActive = false;
};
#pragma once

class Component
{
public:
	Component(const size_t id) : m_id(id) {}
	virtual ~Component() = default;

	inline const size_t GetID() const { return m_id; }

protected:
	size_t m_id;
};
#pragma once

#include <Engine.h>

class PlayerCamera
{
public:
	PlayerCamera();
	~PlayerCamera() = default;

	void Update(Engine::Entity entity);

private:
	bool m_smoothingOn;
	float m_lookAhead;
	float m_lookAheadFactor;
	float m_lookAheadChangeSpeed = 12.8f;

	float m_smoothingFactor;
	float m_rightTargetScreenBound;
	float m_leftTargetScreenBound;
	float m_topTargetScreenBound;
	float m_bottomTargetScreenBound;
};
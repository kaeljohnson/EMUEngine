#pragma once

#include <Engine.h>

class PlayerCamera : public Engine::TargetCamera
{
public:
	PlayerCamera();
	~PlayerCamera() = default;

	void SetTargetSmoothingFactor(const float smoothingFactor);
	void SetRightTargetScreenBound(const float screenBound);
	void SetLeftTargetScreenBound(const float screenBound);
	void SetTopTargetScreenBound(const float screenBound);
	void SetBottomTargetScreenBound(const float screenBound);
	void SetLookAheadFactor(const float lookAhead);

	 void Update(const double interpolation) override;


private:
	bool m_smoothingOn;
	double m_lookAhead;
	double m_lookAheadFactor;
	const float m_lookAheadChangeSpeed = 0.005f;

	float m_smoothingFactor;
	float m_rightTargetScreenBound;
	float m_leftTargetScreenBound;
	float m_topTargetScreenBound;
	float m_bottomTargetScreenBound;

};
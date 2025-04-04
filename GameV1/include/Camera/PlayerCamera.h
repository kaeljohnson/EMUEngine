#pragma once

#include <Engine.h>

class PlayerCamera
{
public:
	PlayerCamera(Engine::Entity entity);
	~PlayerCamera() = default;

	void SetTargetSmoothingFactor(const float smoothingFactor);
	void SetRightTargetScreenBound(const float screenBound);
	void SetLeftTargetScreenBound(const float screenBound);
	void SetTopTargetScreenBound(const float screenBound);
	void SetBottomTargetScreenBound(const float screenBound);
	void SetLookAheadFactor(const float lookAhead);

	// void Update(const double interpolation) override;
	void Update(Engine::Camera& refCamera);


private:
	Engine::Entity m_entity;

	bool m_smoothingOn;
	float m_lookAhead;
	float m_lookAheadFactor;
	float m_lookAheadChangeSpeed = 12.8f;

	float m_smoothingFactor;
	float m_rightTargetScreenBound;
	float m_leftTargetScreenBound;
	float m_topTargetScreenBound;
	float m_bottomTargetScreenBound;

	Engine::TransformInterface& m_refTransformInterface;
	Engine::CameraInterface& m_refCameraInterface;
};
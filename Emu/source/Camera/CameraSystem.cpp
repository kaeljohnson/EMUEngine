#include "../../include/Camera/CameraSystem.h"
#include "../../Public/Screen.h"
#include "../../Public/EMUTime.h"
#include "../../include/ECS/ECS.h"
#include "../../include/Components.h"
#include "../../include/ISDL/ISDL.h"

#include "../../Public/Logger.h"

namespace Engine
{
    CameraSystem::CameraSystem(ECS& refECS) : m_refECS(refECS) {}

	static void clamp(Camera& refCamera)
	{
		const Math2D::Point2D<float> offset = refCamera.m_size / 2.0f;


		// Clamp X
		if (refCamera.m_centerInWorldUnits.X - offset.X < 0)
			refCamera.m_centerInWorldUnits.X = offset.X;

		if (refCamera.m_centerInWorldUnits.X + offset.X > refCamera.m_frameBounds.X)
			refCamera.m_centerInWorldUnits.X = refCamera.m_frameBounds.X - offset.X;

		// Clamp Y
		if (refCamera.m_centerInWorldUnits.Y - offset.Y < 0)
			refCamera.m_centerInWorldUnits.Y = offset.Y;

		if (refCamera.m_centerInWorldUnits.Y + offset.Y > refCamera.m_frameBounds.Y)
			refCamera.m_centerInWorldUnits.Y = refCamera.m_frameBounds.Y - offset.Y;
	}

	static Math2D::Point2D<float> worldToCamera(const Math2D::Point2D<float>& worldPos,
		const Math2D::Point2D<float>& cameraCenter, float parallax, const Math2D::Point2D<float>& halfSize)
	{
		return worldPos - cameraCenter * parallax + halfSize;
	}

	static Math2D::Point2D<int> toPixels(const Math2D::Point2D<float>& units, size_t scaleInPixels)
	{
		return Math2D::Point2D<int>(int(units.X * scaleInPixels), int(units.Y * scaleInPixels));
	}

	static void prepareForRendering(Camera& refCamera, AssetManager& refAssetManager, ECS& refECS,
		const Math2D::Point2D<int> windowSizeInPixels, const int scale)
	{
		auto& renderBuckets = refCamera.m_renderBucket;
#ifndef NDEBUG
		auto& debugBuckets = refCamera.m_debugRenderBucket;
		auto& debugLineBuckets = refCamera.m_debugLinesRenderBucket;
		auto& pointBuckets = refCamera.m_debugPointsRenderBucket;
#endif
		const size_t scaleInPixels = refCamera.m_pixelsPerUnit * scale;
		const Math2D::Point2D<float> halfSize = refCamera.m_size / 2.0f;

		const Math2D::Point2D<float> windowSizeInUnits(
			windowSizeInPixels.X / scaleInPixels,
			windowSizeInPixels.Y / scaleInPixels
		);

		const float rightRenderBound = refCamera.m_viewportSizeInPercentageOfScreen.X * windowSizeInUnits.X + 1.0f;
		const float bottomRenderBound = refCamera.m_viewportSizeInPercentageOfScreen.Y * windowSizeInUnits.Y + 1.0f;

		const float interpolation = Time::GetInterpolationFactor();

		for (Transform& refTransform : refECS.GetHotComponents<Transform>())
		{
			const Math2D::Point2D<float> lerpedPos = refTransform.m_skipLerp ? refTransform.m_position : Math2D::Lerp(refTransform.m_prevPosition, refTransform.m_position, interpolation);

			// refTransform.m_skipLerp = false; // Reset skipLerp after using it for one frame.

			if (Sprite* ptrSpriteComponent = refECS.GetComponent<Sprite>(refTransform.m_entity))
			{
				const Math2D::Point2D<float> objectTopLeft = worldToCamera(
					lerpedPos + ptrSpriteComponent->m_offsetFromTransform,
					refCamera.m_centerInWorldUnits, refTransform.m_parallaxFactor, halfSize);
				const Math2D::Point2D<float> objectBottomRight = objectTopLeft + ptrSpriteComponent->m_sizeInUnits;

				refTransform.m_screenTopLeft = objectTopLeft;			// Temparorly store screen position in Transform for use in other systems (e.g. collision system).
				refTransform.m_screenBottomRight = objectBottomRight;	// The sprite component should store these as well so the client can specify if it wants the transform or the sprite.

				const bool isVisible =
					objectBottomRight.X - 1 >= 0.0f && objectTopLeft.X + 2 <= rightRenderBound &&
					objectBottomRight.Y - 1 >= 0.0f && objectTopLeft.Y + 1 <= bottomRenderBound;

				if (!isVisible)
				{
					refCamera.m_currentFramedEntities.erase(refTransform.m_entity);
					continue;
				}

				refCamera.m_currentFramedEntities.insert(refTransform.m_entity);

				if (ptrSpriteComponent->m_ptrLoadedTexture == nullptr)
					continue;

				const Math2D::Point2D<int> screenPos = toPixels(objectTopLeft, scaleInPixels);
				const Math2D::Point2D<int> screenSize = toPixels(ptrSpriteComponent->m_sizeInUnits, scaleInPixels);

				renderBuckets[refTransform.m_layer].emplace_back(
					refTransform.m_entity,
					screenPos,
					screenSize,
					ptrSpriteComponent->m_locationInPixelsOnSpriteSheet,
					Math2D::Point2D<int>(static_cast<int>(ptrSpriteComponent->m_pixelsPerFrame.X), static_cast<int>(ptrSpriteComponent->m_pixelsPerFrame.Y))
				);
#ifndef NDEBUG
				if (refTransform.m_drawDebug)
				{
					pointBuckets[refTransform.m_layer].emplace_back(
						refTransform.m_entity, screenPos, refTransform.m_debugColor);
				}
				if (ptrSpriteComponent->m_drawDebug)
				{
					debugBuckets[refTransform.m_layer].emplace_back(
						refTransform.m_entity, false, screenPos, screenSize, ptrSpriteComponent->m_debugColor);
				}
#endif
			}

#ifndef NDEBUG
			if (PhysicsBody* ptrPhysicsBody = refECS.GetComponent<PhysicsBody>(refTransform.m_entity))
			{
				const Math2D::Point2D<float> objectTopLeft = worldToCamera(
					refTransform.m_position, refCamera.m_centerInWorldUnits, refTransform.m_parallaxFactor, halfSize);
				const Math2D::Point2D<float> objectBottomRight = objectTopLeft + ptrPhysicsBody->m_dimensions;

				const bool isVisible =
					objectBottomRight.X >= 0.0f && objectTopLeft.X <= rightRenderBound &&
					objectBottomRight.Y >= 0.0f && objectTopLeft.Y <= bottomRenderBound;

				if (!isVisible || !ptrPhysicsBody->m_drawDebug)
					continue;

				debugBuckets[refTransform.m_layer].emplace_back(
					refTransform.m_entity,
					ptrPhysicsBody->m_fillRect,
					toPixels(objectTopLeft, scaleInPixels),
					toPixels(ptrPhysicsBody->m_dimensions, scaleInPixels),
					ptrPhysicsBody->m_debugColor
				);
			}
#endif
		}

#ifndef NDEBUG
		for (auto& chainCollider : refECS.GetHotComponents<ChainCollider>())
		{
			for (auto& edge : chainCollider.m_chain.m_originalEdges)
			{
				const float left = std::min(edge.m_startPoint.X, edge.m_endPoint.X);
				const float right = std::max(edge.m_startPoint.X, edge.m_endPoint.X);
				const float top = std::min(edge.m_startPoint.Y, edge.m_endPoint.Y);
				const float bottom = std::max(edge.m_startPoint.Y, edge.m_endPoint.Y);

				const bool isVisible =
					right >= 0.0f && left <= rightRenderBound &&
					bottom >= 0.0f && top <= bottomRenderBound;

				if (!isVisible)
					continue;

				const Math2D::Point2D<float> a = edge.m_startPoint - refCamera.m_centerInWorldUnits + halfSize;
				const Math2D::Point2D<float> b = edge.m_endPoint - refCamera.m_centerInWorldUnits + halfSize;

				debugLineBuckets[0].emplace_back(-1, toPixels(a, scaleInPixels), toPixels(b, scaleInPixels), DebugColor::Red);
			}
		}
#endif
	}

	void CameraSystem::Update(AssetManager& refAssetManager)
	{
		for (auto& camera : m_refECS.GetHotComponents<Camera>())
		{
			CameraUpdater* ptrCameraUpdater = m_refECS.GetComponent<CameraUpdater>(camera.m_entity);
			if (ptrCameraUpdater)
				ptrCameraUpdater->Update(camera.m_entity);

			if (camera.m_clampingOn) clamp(camera);

			prepareForRendering(camera, refAssetManager, m_refECS, Screen::WINDOW_SIZE, Screen::SCALE);
		}
	}

	void CameraSystem::Frame()
	{
		std::vector<Camera>& activeCameras = m_refECS.GetHotComponents<Camera>();

		if (activeCameras.empty())
		{
			ENGINE_ERROR("No active cameras in the scene. Cannot frame camera.");
			std::exit(1);
		}

		const float ppu = static_cast<float>(Screen::SCALE);
		for (auto& refCamera : activeCameras)
		{
			const Math2D::Point2D<float> viewport(
				Screen::WINDOW_SIZE.X * refCamera.m_viewportSizeInPercentageOfScreen.X,
				Screen::WINDOW_SIZE.Y * refCamera.m_viewportSizeInPercentageOfScreen.Y);
			refCamera.m_size = viewport / (refCamera.m_pixelsPerUnit * ppu);

			if (Transform* ptrTransform = m_refECS.GetComponent<Transform>(refCamera.m_entity))
				refCamera.m_cameraTopLeftInWorldUnits = ptrTransform->m_position - refCamera.m_size / 2.0f;

			clamp(refCamera);
		}
	}
}
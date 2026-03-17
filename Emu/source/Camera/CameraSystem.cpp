#pragma once

#include "../../include/Camera/CameraSystem.h"
#include "../../include/Camera/CameraInterface.h"
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
		if (refCamera.m_offset.X < 0) { refCamera.m_offset.X = 0; }
		if (refCamera.m_offset.X + refCamera.m_size.X > refCamera.m_bounds.X) { refCamera.m_offset.X = refCamera.m_bounds.X - refCamera.m_size.X; }

		if (refCamera.m_offset.Y < 0) { refCamera.m_offset.Y = 0; }
		if (refCamera.m_offset.Y + refCamera.m_size.Y > refCamera.m_bounds.Y) { refCamera.m_offset.Y = refCamera.m_bounds.Y - refCamera.m_size.Y; }
	}

	static void prepareForRendering(Camera& refCamera, AssetManager& refAssetManager, ECS& refECS,
		const Math2D::Point2D<int> windowSizeInPixels, const int scale)
	{
		// auto start = std::chrono::high_resolution_clock::now();

		auto& renderBuckets = refCamera.m_renderBucket;
		auto& debugBuckets = refCamera.m_debugRenderBucket;
		auto& debugLineBuckets = refCamera.m_debugLinesRenderBucket;
		auto& pointBuckets = refCamera.m_debugPointsRenderBucket;

		const size_t scaleInPixels = refCamera.m_pixelsPerUnit * scale;

		// Camera setup
		const Math2D::Point2D<float> windowSizeInUnits(
			windowSizeInPixels.X / scaleInPixels,
			windowSizeInPixels.Y / scaleInPixels
		);

		// Render bound is simply the size of the screen in units
		const float leftRenderBound = 0.0f;
		const float topRenderBound = 0.0f;
		const float rightRenderBound = refCamera.m_viewportSizeInPercentageOfScreen.X * windowSizeInUnits.X + 1.0f;
		const float bottomRenderBound = refCamera.m_viewportSizeInPercentageOfScreen.Y * windowSizeInUnits.Y + 1.0f;

		auto& transformManager = refECS.GetHotComponents<Transform>();
		for (Transform& refTransform : transformManager)
		{
			//Render object construction & submission

			// Get interpolated position of transform.
			const float interpolation = Time::GetInterpolationFactor();
			const float lerpedX = Math2D::Lerp(refTransform.m_prevPosition.X, refTransform.m_position.X, interpolation);
			const float lerpedY = Math2D::Lerp(refTransform.m_prevPosition.Y, refTransform.m_position.Y, interpolation);

			if (Sprite* ptrSpriteComponent = refECS.GetComponent<Sprite>(refTransform.m_entity))
			{
				float offsetFromTransformX = ptrSpriteComponent->m_offsetFromTransform.X;
				float offsetFromTransformY = ptrSpriteComponent->m_offsetFromTransform.Y;

				// 1. Culling
				const float objectLeft = lerpedX + offsetFromTransformX - refCamera.m_offset.X * refTransform.m_parallaxFactor;
				const float objectRight = objectLeft + ptrSpriteComponent->m_sizeInUnits.X;
				const float objectTop = lerpedY + offsetFromTransformY - refCamera.m_offset.Y * refTransform.m_parallaxFactor;
				const float objectBottom = objectTop + ptrSpriteComponent->m_sizeInUnits.Y;

				const bool isVisible =
					objectRight - 1 >= leftRenderBound && objectLeft + 2 <= rightRenderBound &&
					objectBottom - 1 >= topRenderBound && objectTop + 1 <= bottomRenderBound;

				if (!isVisible)
					continue;

				int width = int(ptrSpriteComponent->m_sizeInUnits.X * scaleInPixels);
				int height = int(ptrSpriteComponent->m_sizeInUnits.Y * scaleInPixels);

				SDLTexture* spriteTexture = (SDLTexture*)ptrSpriteComponent->m_ptrLoadedTexture;
				if (spriteTexture == nullptr)
					continue;

				// Sprite sheet coordinates
				const int locationInPixelsOnSpriteSheetX = ptrSpriteComponent->m_locationInPixelsOnSpriteSheet.X;
				const int locationInPixelsOnSpriteSheetY = ptrSpriteComponent->m_locationInPixelsOnSpriteSheet.Y;
				const int sizeInPixelsOnSpriteSheetX = static_cast<int>(ptrSpriteComponent->m_pixelsPerFrame.X);
				const int sizeInPixelsOnSpriteSheetY = static_cast<int>(ptrSpriteComponent->m_pixelsPerFrame.Y);

				// Screen-space coordinates
				const int locationInPixelsOnScreenX =
					int(objectLeft * scaleInPixels);
				const int locationInPixelsOnScreenY =
					int(objectTop * scaleInPixels);

				refTransform.m_positionOnScreen = Math2D::Point2D<int>(locationInPixelsOnScreenX, locationInPixelsOnScreenY);

				renderBuckets[refTransform.m_layer].emplace_back( // No check if index is in bounds. Client needs to make sure all z indices are within 1-10
					refTransform.m_entity,
					Math2D::Point2D<int>(locationInPixelsOnScreenX, locationInPixelsOnScreenY),
					Math2D::Point2D<int>(width, height),
					Math2D::Point2D<int>(locationInPixelsOnSpriteSheetX, locationInPixelsOnSpriteSheetY),
					Math2D::Point2D<int>(sizeInPixelsOnSpriteSheetX, sizeInPixelsOnSpriteSheetY)
				);
#ifndef NDEBUG
				// submit transform origin
				// debug objects when in debug mode.
				if (refTransform.m_drawDebug)
				{
					pointBuckets[refTransform.m_layer].emplace_back(
						refTransform.m_entity,
						Math2D::Point2D<int>(
							int(objectLeft * scaleInPixels),
							int(objectTop * scaleInPixels)
						),
						refTransform.m_debugColor
					);
				}
				if (ptrSpriteComponent->m_drawDebug)
				{
					debugBuckets[refTransform.m_layer].emplace_back(
						refTransform.m_entity,
						false,
						Math2D::Point2D<int>(locationInPixelsOnScreenX, locationInPixelsOnScreenY),
						Math2D::Point2D<int>(width, height),
						ptrSpriteComponent->m_debugColor
					);
				}
#endif
			}

#ifndef NDEBUG
			if (PhysicsBody* ptrPhysicsBody = refECS.GetComponent<PhysicsBody>(refTransform.m_entity))
			{
				const float objectLeft = refTransform.m_position.X - refCamera.m_offset.X * refTransform.m_parallaxFactor;
				const float objectRight = objectLeft + ptrPhysicsBody->m_dimensions.X;
				const float objectTop = refTransform.m_position.Y - refCamera.m_offset.Y * refTransform.m_parallaxFactor;
				const float objectBottom = objectTop + ptrPhysicsBody->m_dimensions.Y;

				const bool isVisible =
					objectRight >= leftRenderBound && objectLeft <= rightRenderBound &&
					objectBottom >= topRenderBound && objectTop <= bottomRenderBound;

				if (!isVisible)
					continue;

				// 2. Position & scale

				if (!ptrPhysicsBody->m_drawDebug)
					continue;

				// submit debug border
				// debug objects when in debug mode.
				debugBuckets[refTransform.m_layer].emplace_back(
					refTransform.m_entity,
					ptrPhysicsBody->m_fillRect,
					Math2D::Point2D<int>(
						int(objectLeft * scaleInPixels),
						int(objectTop * scaleInPixels)
					),
					Math2D::Point2D<int>(
						int((ptrPhysicsBody->m_dimensions.X * scaleInPixels)), // Need transform m_dimensions, not animation m_dimensions
						int((ptrPhysicsBody->m_dimensions.Y * scaleInPixels))
					),
					ptrPhysicsBody->m_debugColor
				);
			}
#endif
		}

#ifndef NDEBUG
		auto submitEdgeForRendering = [&](auto& refEdge)
			{
				// Transform to screen space
				const int edgePointAInPixelsX = static_cast<int>((refEdge.m_startPoint.X - refCamera.m_offset.X) * scaleInPixels);
				const int edgePointAInPixelsY = static_cast<int>((refEdge.m_startPoint.Y - refCamera.m_offset.Y) * scaleInPixels);
				const Math2D::Point2D<int> edgePointAInPixels(edgePointAInPixelsX, edgePointAInPixelsY);
				const int edgePointBInPixelsX = static_cast<int>((refEdge.m_endPoint.X - refCamera.m_offset.X) * scaleInPixels);
				const int edgePointBInPixelsY = static_cast<int>((refEdge.m_endPoint.Y - refCamera.m_offset.Y) * scaleInPixels);
				const Math2D::Point2D<int> edgePointBInPixels(edgePointBInPixelsX, edgePointBInPixelsY);

				debugLineBuckets[0].emplace_back(
					-1,
					edgePointAInPixels,
					edgePointBInPixels,
					DebugColor::Red
				);
			};

		for (auto& chainCollider : refECS.GetHotComponents<ChainCollider>())
		{
			for (auto& edge : chainCollider.m_chain.m_originalEdges)
			{
				float leftMostPoint = std::min(edge.m_startPoint.X, edge.m_endPoint.X);
				float rightMostPoint = std::max(edge.m_startPoint.X, edge.m_endPoint.X);
				float topMostPoint = std::min(edge.m_startPoint.Y, edge.m_endPoint.Y);
				float bottomMostPoint = std::max(edge.m_startPoint.Y, edge.m_endPoint.Y);
				// 1. Culling

				const bool isVisible =
					rightMostPoint >= leftRenderBound && leftMostPoint <= rightRenderBound &&
					bottomMostPoint >= topRenderBound && topMostPoint <= bottomRenderBound;

				if (!isVisible)
					continue;

				submitEdgeForRendering(edge);
			}
		}
#endif
		// auto end = std::chrono::high_resolution_clock::now();
		// std::chrono::duration<double, std::milli> elapsed = end - start;
		// ENGINE_TRACE_D("Camera prepareForRendering took " + std::to_string(elapsed.count()) + " ms");
	}

    void CameraSystem::Update(AssetManager& refAssetManager)
    {
		// auto start = std::chrono::high_resolution_clock::now();
        for (auto& camera : m_refECS.GetHotComponents<Camera>())
        {
            CameraUpdater* ptrCameraUpdater = m_refECS.GetComponent<CameraUpdater>(camera.m_entity);
            if (ptrCameraUpdater)
                ptrCameraUpdater->Update(camera.m_entity);

            if (camera.m_clampingOn) clamp(camera);

			// Prepare this camera for rendering
			prepareForRendering(camera, refAssetManager, m_refECS, Screen::WINDOW_SIZE, Screen::SCALE);
        }
		// auto end = std::chrono::high_resolution_clock::now();
		//std::chrono::duration<double, std::milli> elapsed = end - start;
		// ENGINE_TRACE_D("CameraSystem Update took " + std::to_string(elapsed.count()) + " ms");
    }

    void CameraSystem::Frame(const Math2D::Point2D<int> mapBounds)
    {
		std::vector<Camera>& activeCameras = m_refECS.GetHotComponents<Camera>();

		if (activeCameras.size() == 0)
		{
			ENGINE_ERROR("No active cameras in the scene. Cannot frame camera.");
			std::exit(1);
		}

		for (auto& refCamera : activeCameras)
		{
			refCamera.m_bounds = mapBounds;
			refCamera.m_size
				= Math2D::Point2D<float>((Screen::WINDOW_SIZE.X * refCamera.m_viewportSizeInPercentageOfScreen.X) / (refCamera.m_pixelsPerUnit * Screen::SCALE),
					(Screen::WINDOW_SIZE.Y * refCamera.m_viewportSizeInPercentageOfScreen.Y) / (refCamera.m_pixelsPerUnit * Screen::SCALE));

			// If the entity with the camera has a transform component, center the camera on the transform position
			if (m_refECS.HasComponent<Transform>(refCamera.m_entity))
			{
				Transform* ptrTransform = m_refECS.GetComponent<Transform>(refCamera.m_entity);
				refCamera.m_offset.X = ptrTransform->m_position.X - (refCamera.m_size.X) / 2;
				refCamera.m_offset.Y = ptrTransform->m_position.Y - (refCamera.m_size.Y) / 2;
			}

			clamp(refCamera);
		}
    }
}
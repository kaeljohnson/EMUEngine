#pragma once

#include "../../include/Camera/CameraSystem.h"
#include "../../include/Camera/CameraInterface.h"
#include "../../include/Rendering/Screen.h"
#include "../../include/Time.h"
#include "../../include/ECS/ECS.h"
#include "../../include/Components.h"
#include "../../include/ISDL/ISDL.h"

#include "../../include/Logging/Logger.h"

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
		const Math2D::Point2D<int> viewportSizeInPixels, const Math2D::Point2D<float> scale)
	{
		// auto start = std::chrono::high_resolution_clock::now();

		auto& renderBuckets = refCamera.m_renderBucket;
		auto& debugBuckets = refCamera.m_debugRenderBucket;
		auto& debugLineBuckets = refCamera.m_debugLinesRenderBucket;
		auto& pointBuckets = refCamera.m_debugPointsRenderBucket;

		const float scaleX = refCamera.m_pixelsPerUnit * scale.X;
		const float scaleY = refCamera.m_pixelsPerUnit * scale.Y;

		// Camera setup
		const Math2D::Point2D<float> viewportSizeInTiles(
			viewportSizeInPixels.X / (refCamera.m_pixelsPerUnit * scale.X),
			viewportSizeInPixels.Y / (refCamera.m_pixelsPerUnit * scale.Y)
		);

		// Calculate "adjusted offset". This is the offset that takes
		// into account the position of the camera on the screen.
		const Math2D::Point2D<float> cameraAdjustedOffset(
			refCamera.m_offset.X - refCamera.m_positionInFractionOfScreenSize.X * viewportSizeInTiles.X,
			refCamera.m_offset.Y - refCamera.m_positionInFractionOfScreenSize.Y * viewportSizeInTiles.Y
		);

		// Bounds of what to render in world space. For each camera, 
		// user sees the left side of the camera's world coordinates
		// offset all the way to the width of the camera in world 
		// coordinates. Same for height.
		const float leftRenderBound = refCamera.m_offset.X;
		const float topRenderBound = refCamera.m_offset.Y;
		const float rightRenderBound = leftRenderBound + refCamera.m_screenRatio.X * viewportSizeInTiles.X;
		const float bottomRenderBound = topRenderBound + refCamera.m_screenRatio.Y * viewportSizeInTiles.Y;

		const float renderAreaWidth = rightRenderBound - leftRenderBound;
		const float renderAreaHeight = bottomRenderBound - topRenderBound;

		// Set the render zone.
		refCamera.m_clipRectPosition.X = static_cast<int>(refCamera.m_positionInFractionOfScreenSize.X * viewportSizeInPixels.X);
		refCamera.m_clipRectPosition.Y = static_cast<int>(refCamera.m_positionInFractionOfScreenSize.Y * viewportSizeInPixels.Y);
		refCamera.m_clipRectSize.X = static_cast<int>(renderAreaWidth * refCamera.m_pixelsPerUnit * scale.X);
		refCamera.m_clipRectSize.Y = static_cast<int>(renderAreaHeight * refCamera.m_pixelsPerUnit * scale.Y);

		auto& transformManager = refECS.GetHotComponents<Transform>();
		for (Transform& refTransform : transformManager)
		{
			//Render object construction & submission

			// Get interpolated position of transform.
			const float interpolation = Time::GetInterpolationFactor();
			const float lerpedX = Math2D::Lerp(refTransform.m_prevPosition.X, refTransform.m_position.X, interpolation);
			const float lerpedY = Math2D::Lerp(refTransform.m_prevPosition.Y, refTransform.m_position.Y, interpolation);

			// Separate this into the animation processing system.
			if (Sprite* ptrSpriteComponent = refECS.GetComponent<Sprite>(refTransform.m_entity))
			{
				// 1. Culling
				const float objectLeft = refTransform.m_position.X + ptrSpriteComponent->m_offsetFromTransform.X;
				const float objectRight = objectLeft + ptrSpriteComponent->m_sizeInUnits.X;
				const float objectTop = refTransform.m_position.Y + ptrSpriteComponent->m_offsetFromTransform.Y;
				const float objectBottom = objectTop + ptrSpriteComponent->m_sizeInUnits.Y;

				const bool isVisible =
					objectRight >= leftRenderBound && objectLeft <= rightRenderBound &&
					objectBottom >= topRenderBound && objectTop <= bottomRenderBound;

				if (!isVisible)
					continue;

				int width = int(ptrSpriteComponent->m_sizeInUnits.X * scaleX);
				int height = int(ptrSpriteComponent->m_sizeInUnits.Y * scaleY);

				float offsetFromTransformX = ptrSpriteComponent->m_offsetFromTransform.X;
				float offsetFromTransformY = ptrSpriteComponent->m_offsetFromTransform.Y;

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
					int((lerpedX - cameraAdjustedOffset.X + offsetFromTransformX) * scaleX);
				const int locationInPixelsOnScreenY =
					int((lerpedY - cameraAdjustedOffset.Y + offsetFromTransformY) * scaleY);

				renderBuckets[refTransform.m_zIndex].emplace_back( // No check if index is in bounds. Client needs to make sure all z indices are within 1-10
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
					pointBuckets[refTransform.m_zIndex].emplace_back(
						refTransform.m_entity,
						Math2D::Point2D<int>(
							int((lerpedX - cameraAdjustedOffset.X) * scaleX),
							int((lerpedY - cameraAdjustedOffset.Y) * scaleY)
						),
						refTransform.m_debugColor
					);
				}
				if (ptrSpriteComponent->m_drawDebug)
				{
					debugBuckets[refTransform.m_zIndex].emplace_back(
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
				const float objectLeft = refTransform.m_position.X;
				const float objectRight = objectLeft + ptrPhysicsBody->m_dimensions.X;
				const float objectTop = refTransform.m_position.Y;
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
				debugBuckets[refTransform.m_zIndex].emplace_back(
					refTransform.m_entity,
					ptrPhysicsBody->m_fillRect,
					Math2D::Point2D<int>(
						int((lerpedX - cameraAdjustedOffset.X) * scaleX),
						int((lerpedY - cameraAdjustedOffset.Y) * scaleY)
					),
					Math2D::Point2D<int>(
						int((ptrPhysicsBody->m_dimensions.X * scaleX)), // Need transform m_dimensions, not animation m_dimensions
						int((ptrPhysicsBody->m_dimensions.Y * scaleY))
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
				const int edgePointAInPixelsX = static_cast<int>((refEdge.m_startPoint.X - cameraAdjustedOffset.X) * scaleX);
				const int edgePointAInPixelsY = static_cast<int>((refEdge.m_startPoint.Y - cameraAdjustedOffset.Y) * scaleY);
				const Math2D::Point2D<int> edgePointAInPixels(edgePointAInPixelsX, edgePointAInPixelsY);
				const int edgePointBInPixelsX = static_cast<int>((refEdge.m_endPoint.X - cameraAdjustedOffset.X) * scaleX);
				const int edgePointBInPixelsY = static_cast<int>((refEdge.m_endPoint.Y - cameraAdjustedOffset.Y) * scaleY);
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
			prepareForRendering(camera, refAssetManager, m_refECS, Screen::VIEWPORT_SIZE, Screen::SCALE);
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
			ENGINE_CRITICAL_D("No active cameras in the scene. Cannot frame camera.");
			std::runtime_error("No active cameras in the scene. Cannot frame camera.");
			return;
		}

		for (auto& refCamera : activeCameras)
		{
			refCamera.m_bounds = mapBounds;
			refCamera.m_size
				= Math2D::Point2D<float>((Screen::VIEWPORT_SIZE.X * refCamera.m_screenRatio.X) / (refCamera.m_pixelsPerUnit * Screen::SCALE.X),
					(Screen::VIEWPORT_SIZE.Y * refCamera.m_screenRatio.Y) / (refCamera.m_pixelsPerUnit * Screen::SCALE.Y));

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
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

    void CameraSystem::Update(AssetManager& refAssetManager)
    {
        for (auto& camera : m_refECS.GetHotComponents<Camera>())
        {
            CameraUpdater* ptrCameraUpdater = m_refECS.GetComponent<CameraUpdater>(camera.m_entity);
            if (ptrCameraUpdater)
                ptrCameraUpdater->Update(camera.m_entity);

            if (camera.m_clampingOn) Clamp(camera);

			// Prepare this camera for rendering
			PrepareForRendering(camera, refAssetManager);
        }
    }

	void CameraSystem::PrepareForRendering(Camera& refCamera, AssetManager& refAssetManager)
	{
		
		// Camera setup
			const Vector2D<float> viewportSizeInTiles(
				Screen::VIEWPORT_SIZE.X / (refCamera.m_pixelsPerUnit * Screen::SCALE.X),
				Screen::VIEWPORT_SIZE.Y / (refCamera.m_pixelsPerUnit * Screen::SCALE.Y)
			);

			// Calculate "adjusted offset". This is the offset that takes
			// into account the position of the camera on the screen.
			const Vector2D<float> cameraAdjustedOffset(
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

			ENGINE_CRITICAL_D();
			ENGINE_CRITICAL_D("Render area width: " + std::to_string(renderAreaWidth) + " height: " + std::to_string(renderAreaHeight));
			ENGINE_CRITICAL_D("Render bounds L: " + std::to_string(leftRenderBound) + " R: " + std::to_string(rightRenderBound) +
				" T: " + std::to_string(topRenderBound) + " B: " + std::to_string(bottomRenderBound));

			// Set the render zone.
			refCamera.m_clipRectX = static_cast<int>(refCamera.m_positionInFractionOfScreenSize.X * Screen::VIEWPORT_SIZE.X);
			refCamera.m_clipRectY = static_cast<int>(refCamera.m_positionInFractionOfScreenSize.Y * Screen::VIEWPORT_SIZE.Y);
			refCamera.m_clipRectW = static_cast<int>(renderAreaWidth * refCamera.m_pixelsPerUnit * Screen::SCALE.X);
			refCamera.m_clipRectH = static_cast<int>(renderAreaHeight * refCamera.m_pixelsPerUnit * Screen::SCALE.Y);

			auto& transformManager = m_refECS.GetHotComponents<Transform>();
			for (Transform& refTransform : transformManager)
			{
				// 1. Culling
				const float objectLeft = refTransform.Position.X;
				const float objectRight = objectLeft + refTransform.Dimensions.X;
				const float objectTop = refTransform.Position.Y;
				const float objectBottom = objectTop + refTransform.Dimensions.Y;

				const bool isVisible =
					objectRight >= leftRenderBound && objectLeft <= rightRenderBound &&
					objectBottom >= topRenderBound && objectTop <= bottomRenderBound;

				if (!isVisible)
					continue;

				// 2. Position & scale
				const float interpolation = Time::GetInterpolationFactor();
				const float lerpedX = Lerp(refTransform.PrevPosition.X, refTransform.Position.X, interpolation);
				const float lerpedY = Lerp(refTransform.PrevPosition.Y, refTransform.Position.Y, interpolation);

				const float scaleX = refCamera.m_pixelsPerUnit * Screen::SCALE.X;
				const float scaleY = refCamera.m_pixelsPerUnit * Screen::SCALE.Y;

				int width = static_cast<int>(round(refTransform.Dimensions.X * scaleX));
				int height = static_cast<int>(round(refTransform.Dimensions.Y * scaleY));
				float offsetFromTransformX = 0.0f;
				float offsetFromTransformY = 0.0f;

				Animations* animations = m_refECS.GetComponent<Animations>(refTransform.m_entity);
				if (animations)
				{
					const Animation& currentAnimation = animations->m_animations.at(animations->m_currentAnimation);
					width = static_cast<int>(round(currentAnimation.m_size.X * scaleX));
					height = static_cast<int>(round(currentAnimation.m_size.Y * scaleY));
					offsetFromTransformX = currentAnimation.m_offsetFromTransform.X;
					offsetFromTransformY = currentAnimation.m_offsetFromTransform.Y;
				}

				// 3. Render object construction & submission
				if (animations)
				{
					const Animation& currentAnimation = animations->m_animations.at(animations->m_currentAnimation);

					SDLTexture* spriteTexture = (SDLTexture*)refAssetManager.GetTexture(animations->m_entity);
					if (spriteTexture == nullptr)
						continue;

					// Sprite sheet coordinates
					const int locationInPixelsOnSpriteSheetX =
						static_cast<int>((currentAnimation.m_currentFrame % currentAnimation.m_dimensions.X) *
							currentAnimation.m_pixelsPerFrame.X);
					const int locationInPixelsOnSpriteSheetY =
						static_cast<int>((currentAnimation.m_currentFrame / currentAnimation.m_dimensions.X) *
							currentAnimation.m_pixelsPerFrame.Y);
					const int sizseInPixelsOnSpriteSheetX = static_cast<int>(currentAnimation.m_pixelsPerFrame.X);
					const int sizseInPixelsOnSpriteSheetY = static_cast<int>(currentAnimation.m_pixelsPerFrame.Y);

					// Screen-space coordinates
					const int locationInPixelsOnScreenX =
						static_cast<int>(round((lerpedX - cameraAdjustedOffset.X + offsetFromTransformX) * scaleX));
					const int locationInPixelsOnScreenY =
						static_cast<int>(round((lerpedY - cameraAdjustedOffset.Y + offsetFromTransformY) * scaleY));

					//Submit(std::move(ro));

					auto& bucket = refCamera.m_renderBucket.try_emplace(refTransform.ZIndex).first->second;
					bucket.emplace_back(
						refTransform.m_entity,
						Vector2D<int>(locationInPixelsOnScreenX, locationInPixelsOnScreenY),
						Vector2D<int>(width, height),
						Vector2D<int>(locationInPixelsOnSpriteSheetX, locationInPixelsOnSpriteSheetY),
						Vector2D<int>(sizseInPixelsOnSpriteSheetX, sizseInPixelsOnSpriteSheetY)
					);
				}
			}
	}

    void CameraSystem::Clamp(Camera& refCamera)
    {
        if (refCamera.m_offset.X < 0) { refCamera.m_offset.X = 0; }
        if (refCamera.m_offset.X + refCamera.m_size.X > refCamera.m_bounds.X) { refCamera.m_offset.X = refCamera.m_bounds.X - refCamera.m_size.X; }

        if (refCamera.m_offset.Y < 0) { refCamera.m_offset.Y = 0; }
        if (refCamera.m_offset.Y + refCamera.m_size.Y > refCamera.m_bounds.Y) { refCamera.m_offset.Y = refCamera.m_bounds.Y - refCamera.m_size.Y; }
    }

    void CameraSystem::Frame(Camera& refCamera, const Vector2D<int> mapBounds)
    {
        refCamera.m_bounds = mapBounds;
        refCamera.m_size
            = Vector2D<float>((Screen::VIEWPORT_SIZE.X * refCamera.m_screenRatio.X) / (refCamera.m_pixelsPerUnit * Screen::SCALE.X),
                (Screen::VIEWPORT_SIZE.Y * refCamera.m_screenRatio.Y) / (refCamera.m_pixelsPerUnit * Screen::SCALE.Y));

		// If the entity with the camera has a transform component, center the camera on the transform position
		if (m_refECS.HasComponent<Transform>(refCamera.m_entity))
		{
			Transform* ptrTransform = m_refECS.GetComponent<Transform>(refCamera.m_entity);
			refCamera.m_offset.X = ptrTransform->Position.X - (refCamera.m_size.X) / 2;
			refCamera.m_offset.Y = ptrTransform->Position.Y - (refCamera.m_size.Y) / 2;
		}

		Clamp(refCamera);
    }
}
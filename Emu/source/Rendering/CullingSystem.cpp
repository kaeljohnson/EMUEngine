#include "../../include/Rendering/WindowRenderer.h"
#include "../../include/MathUtil.h"
#include "../../include/Components.h"
#include "../../include/Rendering/Screen.h"
#include "../../include/Time.h"

#include "../../include/ISDL/ISDL.h"
#include "../../include/Logging/Logger.h"

namespace Engine
{

	CullingSystem::CullingSystem(ECS& refECS, AssetManager& refAssetManager) : m_refECS(refECS), m_refAssetManager(refAssetManager) {}

	void CullingSystem::Cull(RenderBucket& refSpriteRenderBucket,
							 DebugRenderBucket& refDebugRenderBucket,
							 LinesRenderBucket& refDebugLinesRenderBucket)
	{
		for (auto& refCamera : m_refECS.GetHotComponents<Camera>())
		{
			if (&refCamera == nullptr)
			{
				ENGINE_CRITICAL_D("No active camera found. Remember to change camera before deactivating entity.");
				throw std::runtime_error("No active camera found.");
				return;
			}



			for (auto& refTransform : m_refECS.GetHotComponents<Transform>())
			{
				const float interpolation = Time::GetInterpolationFactor();

				float lerpedX = Lerp(refTransform.PrevPosition.X, refTransform.Position.X, interpolation);
				float lerpedY = Lerp(refTransform.PrevPosition.Y, refTransform.Position.Y, interpolation);

				const float scaleX = refCamera.m_pixelsPerUnit * Screen::SCALE.X;
				const float scaleY = refCamera.m_pixelsPerUnit * Screen::SCALE.Y;

				int width, height;
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
				else
				{
					width = static_cast<int>(round(refTransform.Dimensions.X * scaleX));
					height = static_cast<int>(round(refTransform.Dimensions.Y * scaleY));
				}

				if (animations)
				{
					const Animation& currentAnimation = animations->m_animations.at(animations->m_currentAnimation);

					SDLTexture* spriteTexture = (SDLTexture*)m_refAssetManager.GetTexture(animations->m_entity);
					if (spriteTexture != nullptr)
					{
						int locationInPixelsOnSpriteSheetX = static_cast<int>(currentAnimation.m_currentFrame % currentAnimation.m_dimensions.X * currentAnimation.m_pixelsPerFrame.X);
						int locationInPixelsOnSpriteSheetY = static_cast<int>(floor(currentAnimation.m_currentFrame / currentAnimation.m_dimensions.X) * currentAnimation.m_pixelsPerFrame.Y);
						int sizseInPixelsOnSpriteSheetX = static_cast<int>(currentAnimation.m_pixelsPerFrame.X);
						int sizseInPixelsOnSpriteSheetY = static_cast<int>(currentAnimation.m_pixelsPerFrame.Y);

						int locationInPixelsOnScreenX = static_cast<int>(round((lerpedX - refCamera.m_screenOffset.X + offsetFromTransformX) * scaleX));
						int locationInPixelsOnScreenY = static_cast<int>(round((lerpedY - refCamera.m_screenOffset.Y + offsetFromTransformY) * scaleY));
						int sizeInPixelsOnScreenX = width;
						int sizeInPixelsOnScreenY = height;

						/*ENGINE_CRITICAL_D("Location in pixels on screen: " + std::to_string(locationInPixelsOnScreenX / (refCamera.m_pixelsPerUnit * Screen::SCALE.X))
							+ ", " + std::to_string(locationInPixelsOnScreenY / refCamera.m_pixelsPerUnit * Screen::SCALE.Y));
						ENGINE_CRITICAL_D("Camera Frame: " + std::to_string(leftFrame) + ", " + std::to_string(topFrame)
							+ ", " + std::to_string(rightFrame) + ", " + std::to_string(bottomFrame));*/

						float objectLeft = refTransform.Position.X;
						float objectRight = objectLeft + refTransform.Dimensions.X;
						float objectTop = refTransform.Position.Y;
						float objectBottom = objectTop + refTransform.Dimensions.Y;

						bool isVisible = objectRight >= refCamera.m_offset.X && objectLeft <= refCamera.m_bottomRightCorner.X &&
							objectBottom >= refCamera.m_offset.Y && objectTop <= refCamera.m_bottomRightCorner.Y;

						if (isVisible)
						{
							auto& bucket = refSpriteRenderBucket.try_emplace(refTransform.ZIndex).first->second;
							bucket.emplace_back(
								// RenderObject
								//{
								refTransform.m_entity,
								Vector2D<int>(locationInPixelsOnScreenX, locationInPixelsOnScreenY),
								Vector2D<int>(sizeInPixelsOnScreenX, sizeInPixelsOnScreenY),
								Vector2D<int>(locationInPixelsOnSpriteSheetX, locationInPixelsOnSpriteSheetY),
								Vector2D<int>(sizseInPixelsOnSpriteSheetX, sizseInPixelsOnSpriteSheetY)
								//}
							);
						}
					}
				}
				else
				{
					float objectLeft = refTransform.Position.X;
					float objectRight = objectLeft + refTransform.Dimensions.X;
					float objectTop = refTransform.Position.Y;
					float objectBottom = objectTop + refTransform.Dimensions.Y;

					bool isVisible = objectRight >= refCamera.m_offset.X && objectLeft <= refCamera.m_bottomRightCorner.X &&
						objectBottom >= refCamera.m_offset.Y && objectTop <= refCamera.m_bottomRightCorner.Y;

					if (isVisible)
					{
						auto& bucket = refDebugRenderBucket.try_emplace(refTransform.ZIndex).first->second;
						bucket.emplace_back(
							// DebugRect
							//{
							refTransform.m_entity,
							false,
							Vector2D<int>(static_cast<int>(round((lerpedX - refCamera.m_screenOffset.X + offsetFromTransformX) * scaleX)),
								static_cast<int>(round((lerpedY - refCamera.m_screenOffset.Y + offsetFromTransformY) * scaleY))),
							Vector2D<int>(static_cast<int>(round(refTransform.Dimensions.X * scaleX)),
								static_cast<int>(round(refTransform.Dimensions.Y * scaleY)))
							//}
						);

					}
				}
			}
		}
	}
}
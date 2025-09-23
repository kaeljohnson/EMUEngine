#pragma once

#include "../ECS/ECS.h"
#include "../Includes.h"
#include "../MathUtil.h"
#include "../Components.h"
#include "../AssetManager.h"
#include "Screen.h"

namespace Engine
{
	struct RenderObject
	{
		RenderObject(size_t entity, Vector2D<int> locationInPixelsOnScreen, Vector2D<int> sizeInPixelsOnScreen, Vector2D<int> locationInPixelsOnSpriteSheet, Vector2D<int> sizeInPixelsOnSpriteSheet)
			: m_entity(entity), m_locationInPixelsOnScreen(locationInPixelsOnScreen), m_sizeInPixelsOnScreen(sizeInPixelsOnScreen),
			m_locationInPixelsOnSpriteSheet(locationInPixelsOnSpriteSheet), m_sizeInPixelsOnSpriteSheet(sizeInPixelsOnSpriteSheet) {}

		size_t m_entity;
		Vector2D<int> m_locationInPixelsOnScreen;
		Vector2D<int> m_sizeInPixelsOnScreen;
		Vector2D<int> m_locationInPixelsOnSpriteSheet;
		Vector2D<int> m_sizeInPixelsOnSpriteSheet;
	};

	struct DebugRect
	{
		DebugRect(size_t entity, bool filled, Vector2D<int> locationInPixelsOnScreen, Vector2D<int> sizeInPixelsOnScreen)
			: m_entity(entity), m_filled(filled), m_locationInPixelsOnScreen(locationInPixelsOnScreen), m_sizeInPixelsOnScreen(sizeInPixelsOnScreen) {}

		size_t m_entity;
		bool m_filled;
		Vector2D<int> m_locationInPixelsOnScreen;
		Vector2D<int> m_sizeInPixelsOnScreen;
	};

	using RenderBucket = std::map<size_t, std::vector<RenderObject>>;
	using DebugRenderBucket = std::map<size_t, std::vector<DebugRect>>;
	using LinesRenderBucket = std::map<size_t, std::vector<Vector2D<int>>>;

	// Simple class that probably does not need to be a class. Could just be a cull function that takes ref
	// to ecs, and asset manager.
	class CullingSystem
	{
	public:

		CullingSystem(ECS& refECS, AssetManager& refAssetManager);
		void Cull(RenderBucket& m_spriteRenderBucket,
				  DebugRenderBucket& m_debugRenderBucket,
				  LinesRenderBucket& m_debugLinesRenderBucket);

	private:

		ECS& m_refECS;
		AssetManager& m_refAssetManager;
	};

	class WindowRenderer
	{
	public:
		WindowRenderer(ECS& refECS, AssetManager& refAssetManager);
		~WindowRenderer();

		void Initialize();

		void SetViewport();
		void ToggleFullscreen();

		const float GetMonitorRefreshRate();

		void Render();
		void Draw(Transform& refTransform, Animations* ptrAnimations, const int pixelsPerUnit, const Vector2D<float> offset);
		void Draw(ChainCollider& transform, const int pixelsPerUnit, const Vector2D<float> offset);
		void Draw(RenderObject& object, const int pixelsPerUnit);
		void DebugDraw(DebugRect& refRenderObject, const int pixelsPerUnit);
		void Display();

		void ClearScreen();

		void free();

	public:
		void* m_ptrWindow;
		void* m_ptrRenderer;

		bool m_rendererCreated;

		std::map<size_t, std::vector<RenderObject>> m_spriteRenderBucket;		// All the sprites to render, in order of layer (highest zIndex to lowest)
		std::map<size_t, std::vector<DebugRect>> m_debugRenderBucket;		// All the debug objects to render, in order of layer (highest zIndex to lowest)
		std::map<size_t, std::vector<Vector2D<int>>> m_debugLinesRenderBucket;	// All the debug lines to render, in order of layer (highest zIndex to lowest)


		ECS& m_refECS;
		AssetManager& m_refAssetManager;

		std::vector<Entity> m_sortedEntitiesToRender;
	};
}
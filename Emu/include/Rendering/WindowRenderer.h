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
		RenderObject(const size_t entity, const Vector2D<size_t> pixelsPerFrame, const Vector2D<float> size, const Vector2D<float> offsetFromTransform, 
			const Vector2D<int> locationInPixelsOnSpriteSheet, const Vector2D<int> locationInPixelsOnScreen)
			: m_entity(entity), m_pixelsPerFrame(pixelsPerFrame), m_sizeInUnits(size), 
			m_offsetFromTransform(offsetFromTransform), m_locationInPixelsOnSpriteSheet(locationInPixelsOnSpriteSheet), m_locationInPixelsOnScreen(locationInPixelsOnScreen) {}
		size_t m_entity;
		Vector2D<size_t> m_pixelsPerFrame;
		Vector2D<float> m_sizeInUnits;
		Vector2D<float> m_offsetFromTransform;
		Vector2D<int> m_locationInPixelsOnSpriteSheet;
		Vector2D<int> m_locationInPixelsOnScreen;
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

		void Submit(const size_t entity, int zIndex, 
			const Vector2D<size_t> pixelSize, const Vector2D<float> size, 
			const Vector2D<float> offsetFromTransform, Vector2D<int> locationInPixelsOnSpriteSheet,
			Vector2D<int> locationInPixelsOnScreen);

		void Render();
		void Draw(Transform& refTransform, Animations* ptrAnimations, const int pixelsPerUnit, const Vector2D<float> offset);
		void Draw(ChainCollider& transform, const int pixelsPerUnit, const Vector2D<float> offset);
		void Draw(RenderObject renderObject);
		void Display();

		void ClearScreen();

		void free();

	public:
		void* m_ptrWindow;
		void* m_ptrRenderer;

		bool m_rendererCreated;

		ECS& m_refECS;
		AssetManager& m_refAssetManager;

		std::vector<Entity> m_sortedEntitiesToRender;

		// zIndex, objectData
		std::unordered_map<size_t, std::vector<RenderObject>> m_entitiesToRender;
	};

	class RendererSystem
	{
	public:
		RendererSystem(ECS& refECS, WindowRenderer& refRenderer); // Pass a callback function that wraps the Submit function here instead of entire renderer
		~RendererSystem() = default;

		void Update();

	private:
		WindowRenderer& m_refRenderer;
		ECS& m_refECS;
	};
}
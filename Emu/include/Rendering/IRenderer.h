#pragma once

#include "../ECS/ECS.h"
#include "../Includes.h"
#include "../Components.h"
#include "../AssetManager.h"

namespace Engine
{
	/**
	* @class IRenderer
	* 
	* @brief Interface for rendering systems.
	*/
	class IRenderer
	{
	public:
		/**
		* @brief Constructor for the IRenderer class.
		* 
		* @param refECS Reference to the ECS instance.
		* @param refAssetManager Reference to the AssetManager instance.
		*/
		IRenderer(ECS& refECS, AssetManager& refAssetManager);
		~IRenderer();

		/**
		* @brief Toggle full screen mode.
		*/
		void ToggleFullscreen();

		/**
		* @brief Get the monitor refresh rate.
		* 
		* @return The monitor refresh rate as a float.
		*/
		const float GetMonitorRefreshRate();

		/**
		* @brief Check if a window resize has been requested.
		*/
		void CheckForWindowResizeRequest();

		/**
		* @brief Render the current scene.
		*/
		void Render();

		/**
		* @brief Get the position of the window.
		* 
		* @return The position of the window as a Point2D<float>.
		*/
		const Math2D::Point2D<int> GetWindowPosition();
		
	private:

		/**
		* @brief Draw Render objects to the screen. Render objects are
		* associated with a texture.
		* 
		* @param object The RenderObject to draw.
		*/
		void draw(RenderObject& object);

		/**
		* @brief Draw Debug objects to the screen. Debug objects are
		* simple shapes drawn with solid colors.
		* 
		* @param object The DebugObject to draw.
		*/
		void draw(DebugObject& object);

		/**
		* @brief Draw Line objects to the screen. Line objects are
		* simple lines drawn with solid colors.
		* 
		* @param line The LineObject to draw.
		*/
		void draw(LineObject& line);

		/**
		* @brief Draw Point objects to the screen. Point objects are
		* simple points drawn with solid colors.
		*/
		void draw(DebugPointObject& point);

		/**
		* @brief Display the rendered objects to the screen.
		*/
		void display();

		/**
		* @brief Set the viewport dimensions based on the current window size
		*/
		void setWindowScale();

		/**
		* @brief Clear the screen before rendering.
		*/
		void clearScreen();

		/**
		* @brief Free renderer resources.
		*/
		void free();

		void* m_ptrWindow;					/// Pointer to the window instance.
		void* m_ptrRenderer;				/// Pointer to the renderer instance.

		bool m_rendererCreated;				/// Flag to indicate if the renderer was successfully created.

		DebugColor m_lastDebugColor;		/// Store the last used debug color to minimize state changes.

		ECS& m_refECS;;						/// Reference to the ECS instance.
		AssetManager& m_refAssetManager;	/// Reference to the AssetManager instance.
	};
}
#pragma once

#include "../MathUtil.h"

namespace Engine
{
	/**
	* @struct Screen
	* 
	* @brief Interface for client to set window size and fullscreen along with other misc screen properties
	* 
	* @note There is various terminology referring to similar concepts in this class. 
	* When screen is used, it is referring generally to the window or display as a whole.
	* the window is what the user sees on their monitor, whether in fullscreen or windowed mode.
	* It is the canvas onto which things are drawn.
	* Viewport refers to a subsection of the screen that has its own rendering context.
	* 
	* @todo Should this class be un-staticified and given a pointer to the window?
	*/
	struct Screen
	{
		/**
		* @brief Get various screen properties
		* 
		* @return Various screen properties
		*/
		inline static const Math2D::Point2D<int> GetScreenSize() { return DISPLAY_RESOLUTION; }

		/**
		* @brief Get virtual screen properties
		* 
		* @return Various virtual screen properties
		*/
		inline int GetVirtualSize() { return VIRTUAL_HEIGHT; }

		/**
		* @brief Get scale properties
		* 
		* @return scale
		*/
		inline static const int GetScale() { return SCALE; }

		/**
		* @brief Get window properties	
		* 
		* @return Window size.
		*/
		inline static const Math2D::Point2D<int> GetWindowSize() { return WINDOW_SIZE; }

		/**
		* @brief Set window size
		*
		* @param size New window size
		*/
		inline static void SetWindowSize(const Math2D::Point2D<int>& size) { WINDOW_SIZE = size; WINDOW_RESIZE_REQUEST = true; }

		/**
		* @brief Set fullscreen mode
		*/
		inline static void SetFullscreen() { TOGGLE_FULLSCREEN_REQUEST = true; }

	private:

		static bool WINDOW_RESIZE_REQUEST;				/// Request to resize the window
		static bool TOGGLE_FULLSCREEN_REQUEST;			/// Request to toggle fullscreen mode

		/**
		* @brief Display resolution is the current resolution of the window or fullscreen display.
		*/
		static Math2D::Point2D<int> DISPLAY_RESOLUTION;

		/**
		* @brief Virtual height is the resolution the simulation is based on before being scaled to the display resolution.
		* The current implementation maintains a fixed height of 720 "virtual pixels" and adjusts the width based on the display aspect ratio.
		*/
		static int VIRTUAL_HEIGHT;

		/**
		* @brief Scale is the factor between the virtual resolution and the display resolution. Scale is
		* set based on the height of the display resolution to maintain consistent vertical scaling.
		*/
		static int SCALE;

		/**
		* @brief Window size is the pixel size of the window on the screen.
		*/
		static Math2D::Point2D<int> WINDOW_SIZE;

		friend class IRenderer;							/// Renderer needs access to the private members to handle window resizing and fullscreen toggling
		friend class CameraInterface;					/// Camera interface needs access to screen properties
		friend class CameraSystem;						/// Camera system needs access to screen properties
	};
}
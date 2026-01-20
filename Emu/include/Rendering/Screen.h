#pragma once

#include "../MathUtil.h"

namespace Engine
{
	/**
	* @struct Screen
	* 
	* @brief Interface for client to set window size and fullscreen along with other misc screen properties
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
		inline static const Math2D::Point2D<int> GetVirtualSize() { return VIRTUAL_SIZE; }

		/**
		* @brief Get scale properties
		* 
		* @return Various scale properties
		*/
		inline static const Math2D::Point2D<float> GetScale() { return SCALE; }

		/**
		* @brief Get scale constant
		* 
		* @return Scale constant
		*/
		inline static const float GetScaleConstant() { return SCALE_CONSTANT; }

		/**
		* @brief Get viewport properties	
		* 
		* @return Various viewport properties
		*/
		inline static const Math2D::Point2D<int> GetViewportSize() { return VIEWPORT_SIZE; }

		/**
		* @brief Get viewport position
		* 
		* @return Viewport position
		*/
		inline static const Math2D::Point2D<int> GetViewportPosition() { return VIEWPORT_POSITION; }

		/**
		* @brief Set various screen properties
		*
		* @param size New window size
		*/
		inline static void SetWindowSize(const Math2D::Point2D<int>& size) { VIEWPORT_SIZE = size; WINDOW_RESIZE_REQUEST = true; }

		/**
		* @brief Set fullscreen mode
		*/
		inline static void SetFullscreen() { TOGGLE_FULLSCREEN_REQUEST = true; }

	private:

		static bool WINDOW_RESIZE_REQUEST;				/// Request to resize the window
		static bool TOGGLE_FULLSCREEN_REQUEST;			/// Request to toggle fullscreen mode

		static Math2D::Point2D<int> DISPLAY_RESOLUTION; /// The current display resolution
		static Math2D::Point2D<int> VIRTUAL_SIZE;		/// The virtual resolution the game is rendered at
		static Math2D::Point2D<float> SCALE;			/// The scale between the display resolution and virtual resolution
		static float SCALE_CONSTANT;					/// The scale constant used for scaling objects

		static Math2D::Point2D<int> VIEWPORT_SIZE;		/// The size of the viewport within the window
		static Math2D::Point2D<int> VIEWPORT_POSITION;	/// The position of the viewport within the window

		friend class IRenderer;							/// Renderer needs access to the private members to handle window resizing and fullscreen toggling
		friend class CameraInterface;					/// Camera interface needs access to viewport properties
		friend class CameraSystem;						/// Camera system needs access to viewport properties
	};
}
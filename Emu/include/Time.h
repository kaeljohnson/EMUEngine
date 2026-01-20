#pragma once

namespace Engine
{
	struct Time
	{
		/**
		* @brief Gets the interpolation factor used for rendering between physics updates.
		* 
		* @return The interpolation factor.
		*/
		inline static const float GetInterpolationFactor() { return INTERPOLATION_FACTOR; }

		/**
		* @brief Gets the fixed time step used for physics updates.
		* 
		* @return The time step.
		*/
		inline static const float GetTimeStep() { return TIME_STEP; }

		/**
		* @brief Sets the interpolation factor used for rendering between physics updates.
		* 
		* @param interpolationFactor The interpolation factor.
		*/
		inline static void SetInterpolationFactor(const float interpolationFactor) { INTERPOLATION_FACTOR = interpolationFactor; }

		/**
		* @brief Sets the fixed time step used for physics updates.
		* 
		* @param timeStep The time step.
		*/
		inline static void SetTimeStep(const float timeStep) { TIME_STEP = timeStep; }

		/**
		* @brief Sets whether the application is currently running.
		* 
		* @param appRunning True if the application is running, false otherwise.
		*/
		inline static void SetAppRunning(const bool appRunning) { APP_RUNNING = appRunning; }

		/**
		* @brief Gets whether the application is currently running.
		* 
		* @return True if the application is running, false otherwise.
		*/
		inline static const bool IsAppRunning() { return APP_RUNNING; }
	private:
		static float INTERPOLATION_FACTOR; /// The interpolation factor for rendering between physics updates.
		static float TIME_STEP;			   /// The fixed time step for physics updates.
		static bool APP_RUNNING;		   /// Is the application currently running?
	};

	inline float Time::INTERPOLATION_FACTOR = 0.0f;
	inline float Time::TIME_STEP = 1.0f / 60.0f;
	inline bool Time::APP_RUNNING = false;
}
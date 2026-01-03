#pragma once

#include "../ECS/ECS.h"
#include "../AssetManager.h"
#include "../MathUtil.h"

namespace Engine
{
	/**
	* @brief Process and manager cameras during runtime.
	* 
	*/
	class CameraSystem
	{
	public:
		CameraSystem(ECS& refECS);
		~CameraSystem() = default;

		/**
		* @brief Frame the camera to an area defined by mapBounds.
		* 
		* * @param mapBounds The bounds of the map to frame the camera within.
		*/
		void Frame(const Math2D::Point2D<int> mapBounds);

		/**
		* @brief Updates all cameras in the system.
		* 
		* * @param refAssetManager Reference to the Asset Manager instance.
		*/
		void Update(AssetManager& refAssetManager);

	private:
		ECS& m_refECS;
	};
}
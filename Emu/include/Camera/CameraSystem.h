#pragma once

#include "../ECS/ECS.h"
#include "../AssetManager.h"
#include "../../Public/MathUtil.h"

namespace Engine
{
	/**
	* @brief Process and manager cameras during runtime.
	* 
	*/
	class CameraSystem
	{
	public:
		/**
		* @brief Constructs the CameraSystem with a reference to the ECS.
		*
		* @param refECS Reference to the Entity Component System.
		*/
		CameraSystem(ECS& refECS);

		/**
		* @brief Default destructor.
		*/
		~CameraSystem() = default;

		/**
		* @brief Frame the camera to be centered on its entity and set its bounds to the map bounds.
		*/
		void Frame();

		/**
		* @brief Updates all cameras in the system.
		* 
		* @param refAssetManager Reference to the Asset Manager instance.
		*/
		void Update(AssetManager& refAssetManager);

	private:
		ECS& m_refECS;
	};
}
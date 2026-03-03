#pragma once

namespace Engine
{
	/**
	* @enum All the possible engine supported body types.
	*/
	enum BodyType
	{
		STATIC,		/// Immovable object.
		DYNAMIC,	/// Movable object affected by forces.
		KINEMATIC,	/// Movable object not affected by forces.
		SENSOR		/// Non-collidable trigger object.
	}; 

	/**
	* @enum Collision filter types.
	*/
	enum Filter
	{
		NONE = 0x00000000,		 /// No collision.
		PLAYER = 0x00000002, 	 /// Player collision.
		MAP = 0x00000004,		 /// Map collision.
		ALL = 0xFFFFFFFF		 /// Collide with all.
	};
}
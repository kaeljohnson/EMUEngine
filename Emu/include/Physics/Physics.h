#pragma once

#include "../ECS/Entity.h"
#include "../Core.h"
#include "../MathUtil.h"

#include "../EventListeners.h"

struct b2WorldId;

namespace Engine
{
	class Physics
	{
	public:
		// PhysicsBody2d getter and setter wrappers
		EMU_API static void SetGravity(Entity* ptrEntity, bool enabled);
		EMU_API static void SetPosition(Entity* ptrEntity, const Vector2D<float> position);
		EMU_API static const Vector2D<float> GetPosition(Entity* ptrEntity);
		EMU_API static const Vector2D<float> GetTopLeftPosition(Entity* ptrEntity);
		EMU_API static void ApplyForceToBody(Entity* ptrEntity, Vector2D<float> force);
		EMU_API static void ApplyImpulseToBody(Entity* ptrEntity, Vector2D<float> impulse);
		EMU_API static void SetVelocity(Entity* ptrEntity, const Vector2D<float> velocity); 
		EMU_API static void SetXVelocity(Entity* ptrEntity, const float xVelocity);
		EMU_API static void SetYVelocity(Entity* ptrEntity, const float yVelocity);
		EMU_API static void SetDeceleration(Entity* ptrEntity, const float decel);
		EMU_API static const Vector2D<float> GetVelocity(Entity* ptrEntity);
		EMU_API static void SetRestitution(Entity* ptrEntity, const float restitution);
		EMU_API static void SetDensity(Entity* ptrEntity, const float density);
		EMU_API static void SetFriction(Entity* ptrEntity, const float friction);
		EMU_API static void SetFixedRotation(Entity* ptrEntity, bool fixed);
		EMU_API static const float GetAngleInRadians(Entity* ptrEntity);
		EMU_API static const float GetAngleInDegrees(Entity* ptrEntity);

		static void CreateWorld(const Vector2D<float> gravity);
		static void AddPhysicsBodiesToWorld();

		// PhysicsBody2D specific functions
		// Call sparingly!!!
		static void RemoveBodyFromWorld(Entity* ptrEntity);
		static void DestroyWorld();

		static void Update();
		static void ProcessContactEvents();

		// Should these be in the event system instead?
		EMU_API static void RegisterOnBeginContactEventListener(SingleEntityContactKey, std::function<void(BeginContact)> callback);
		EMU_API static void RegisterOnEndContactEventListener(SingleEntityContactKey, std::function<void(EndContact)> callback);
		EMU_API static void RegisterOnBeginSensingEventListener(SingleEntityContactKey, std::function<void(BeginSensing)> callback);
		EMU_API static void RegisterOnEndSensingEventListener(SingleEntityContactKey, std::function<void(EndSensing)> callback);
		EMU_API static void RegisterOnBeginContactEventListener(MultiEntityContactKey, std::function<void(BeginContact)> callback);
		EMU_API static void RegisterOnEndContactEventListener(MultiEntityContactKey, std::function<void(EndContact)> callback);
		EMU_API static void RegisterOnBeginSensingEventListener(MultiEntityContactKey, std::function<void(BeginSensing)> callback);
		EMU_API static void RegisterOnEndSensingEventListener(MultiEntityContactKey, std::function<void(EndSensing)> callback);
	private:
		static b2WorldId* m_ptrWorldId;

		static std::unordered_map<SingleEntityContactKey, std::function<void(BeginContact)>> m_onBeginContactSingleEntityMap;
		static std::unordered_map<SingleEntityContactKey, std::function<void(EndContact)>> m_onEndContactSingleEntityMap;
		static std::unordered_map<SingleEntityContactKey, std::function<void(BeginSensing)>> m_onBeginSensingSingleEntityMap;
		static std::unordered_map<SingleEntityContactKey, std::function<void(EndSensing)>> m_onEndSensingSingleEntityMap;

		static std::unordered_map<MultiEntityContactKey, std::function<void(BeginContact)>> m_onBeginContactMultiEntityMap;
		static std::unordered_map<MultiEntityContactKey, std::function<void(EndContact)>> m_onEndContactMultiEntityMap;
		static std::unordered_map<MultiEntityContactKey, std::function<void(BeginSensing)>> m_onBeginSensingMultiEntityMap;
		static std::unordered_map<MultiEntityContactKey, std::function<void(EndSensing)>> m_onEndSensingMultiEntityMap;
	};
}
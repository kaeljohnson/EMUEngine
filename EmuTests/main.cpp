#pragma once

#include <iostream>
#include <chrono>
#include "Engine.h"
#include <gtest/gtest.h>


struct TestComponent : public Engine::Component
{
    TestComponent(Engine::Entity entity, int value) : m_value(value), Component(entity) {}

    size_t m_value;
};

struct TestComponent1 : public Engine::Component
{
	TestComponent1(Engine::Entity entity, int value) : m_value(value), Component(entity) {}

	size_t m_value;
};

static Engine::ECS smallECS;
static Engine::ECS tenEntityECS;
static Engine::ECS mediumECS;
static Engine::ECS largeECS;
static std::vector<Engine::Entity> entitiesForSparseSet;

TEST(ECS, Initialize)
{
	smallECS.Initialize(1);
	tenEntityECS.Initialize(10);
	mediumECS.Initialize(1000);
	largeECS.Initialize(10000);
}

// Sample test case
TEST(ECS, CreateEntityTest) 
{
	Engine::Entity entity = smallECS.CreateEntity();
	EXPECT_EQ(entity, 0);
}

TEST(ECS, HasEntity)
{
	EXPECT_TRUE(smallECS.HasEntity(0));
	EXPECT_FALSE(smallECS.HasEntity(1));
}

TEST(ECS, CreateMultipleEntities)
{
	bool allIDsCorrect = true;

	for (size_t idx = 0; idx < 1000; ++idx)
	{
		Engine::Entity entity = mediumECS.CreateEntity();

		EXPECT_EQ(entity, idx);
	}

	EXPECT_EQ(mediumECS.GetNumEntities(), 1000);
}

TEST(ECS, TooManyEntities)
{
	try
	{
		Engine::Entity invalidEntity = smallECS.CreateEntity();
	}
	catch (const std::runtime_error& e)
	{
		EXPECT_STREQ(e.what(), "Error: All IDs are taken.");
	}

	EXPECT_EQ(smallECS.GetNumEntities(), 1);
}

TEST(ECS, ComponentManagerRegistration)
{
	EXPECT_FALSE(smallECS.HasComponentManager<TestComponent>());
	EXPECT_FALSE(tenEntityECS.HasComponentManager<TestComponent>());
	EXPECT_FALSE(mediumECS.HasComponentManager<TestComponent>());
	EXPECT_FALSE(largeECS.HasComponentManager<TestComponent>());

	smallECS.RegisterComponentManager<TestComponent>();
	tenEntityECS.RegisterComponentManager<TestComponent>();
	mediumECS.RegisterComponentManager<TestComponent>();
	largeECS.RegisterComponentManager<TestComponent>();

	EXPECT_TRUE(smallECS.HasComponentManager<TestComponent>());
	EXPECT_TRUE(tenEntityECS.HasComponentManager<TestComponent>());
	EXPECT_TRUE(mediumECS.HasComponentManager<TestComponent>());
	EXPECT_TRUE(largeECS.HasComponentManager<TestComponent>());
}

TEST(ECS, ComponentAdditionWithoutCorrespondingEntity)
{
	try
	{
		tenEntityECS.AddComponent<TestComponent>(1, 42);
	}
	catch (const std::runtime_error& e)
	{
		EXPECT_STREQ(e.what(), "Error: Entity does not exist.");
	}


	EXPECT_FALSE(tenEntityECS.HasComponent<TestComponent>(1));
	
}

TEST(ECS, AddComponent)
{
	smallECS.AddComponent<TestComponent>(0, 42);
	EXPECT_TRUE(smallECS.HasComponent<TestComponent>(0));
}

TEST(ECS, AddTwoComponentsToSameEntity)
{
	try
	{
		smallECS.AddComponent<TestComponent>(0, 42);
	}
	catch (const std::runtime_error& e)
	{
		EXPECT_STREQ(e.what(), "Error: Component already exists.");
	}
}

TEST(ECS, ComponentActivationAndDeactivation)
{

	auto start = std::chrono::high_resolution_clock::now();
	smallECS.Activate<TestComponent>(0);
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Activation: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

	EXPECT_TRUE(smallECS.IsActive<TestComponent>(0));

	start = std::chrono::high_resolution_clock::now();
	smallECS.Deactivate<TestComponent>(0);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Deactivation: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
	EXPECT_FALSE(smallECS.IsActive<TestComponent>(0));
}

TEST(ECS, TestComponentRetrieval)
{
	EXPECT_EQ(smallECS.GetComponent<TestComponent>(0)->m_value, 42);
}

TEST(ECS, TestComponentDestruction)
{
	smallECS.DestroyComponent<TestComponent>(0);
	EXPECT_FALSE(smallECS.HasComponent<TestComponent>(0));
}

//TEST(ECS, TestSparseArrayIteration)
//{
//	for (size_t i = 0; i < 10000; ++i)
//	{
//		largeECS.CreateEntity();
//		largeECS.AddComponent<TestComponent>(i, i);
//		largeECS.Activate(i);
//		entitiesForSparseSet.push_back(i);
//	}
//
//	EXPECT_EQ(largeECS.GetNumActiveComponents<TestComponent>(), 10000);
//
//	auto start = std::chrono::high_resolution_clock::now();
//	for (Engine::Entity& entity : entitiesForSparseSet)
//	{
//		TestComponent* component = largeECS.GetComponent<TestComponent>(entity);
//		EXPECT_EQ(component->m_value, component->m_entity);
//	}
//	auto end = std::chrono::high_resolution_clock::now();
//	std::cout << "Sparse set iteration: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
//}

TEST(ECS, TestDenseArrayIteration)
{
	auto start = std::chrono::high_resolution_clock::now();
	auto& hotComponents = largeECS.GetHotComponents<TestComponent>();

	for (TestComponent& component : hotComponents)
	{
		EXPECT_EQ(component.m_value, component.m_entity);
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Dense set iteration: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
}

TEST(ECS, TestComponentRearranging)
{
	for (size_t i = 0; i < 10; ++i)
	{
		Engine::Entity entity = tenEntityECS.CreateEntity();
		tenEntityECS.AddComponent<TestComponent>(entity, i+10);
		tenEntityECS.Activate(entity);
	}

	EXPECT_EQ(tenEntityECS.GetNumActiveComponents<TestComponent>(), 10);

	std::vector<TestComponent> components = 
	{ TestComponent(0, 10), TestComponent(1, 11), TestComponent(2, 12), 
	  TestComponent(3, 13), TestComponent(4, 14), TestComponent(5, 15), 
	  TestComponent(6, 16), TestComponent(7, 17), TestComponent(8, 18), 
	  TestComponent(9, 19) };

	for (size_t idx = 0; idx < 10; ++idx)
	{
		EXPECT_EQ(tenEntityECS.GetHotComponents<TestComponent>()[idx].m_entity, components[idx].m_entity);
		EXPECT_EQ(tenEntityECS.GetHotComponents<TestComponent>()[idx].m_value, components[idx].m_value);

		// Check sparse set.
		EXPECT_EQ(tenEntityECS.GetComponent<TestComponent>(components[idx].m_entity)->m_value, components[idx].m_value);
		EXPECT_EQ(tenEntityECS.GetComponent<TestComponent>(components[idx].m_entity)->m_entity, components[idx].m_entity);
	}

	tenEntityECS.Deactivate(3);

	components = { TestComponent(0, 10), TestComponent(1, 11), TestComponent(2, 12), 
				   TestComponent(9, 19), TestComponent(4, 14), TestComponent(5, 15), 
				   TestComponent(6, 16), TestComponent(7, 17), TestComponent(8, 18) };

	for (size_t idx = 0; idx < 9; ++idx)
	{
		EXPECT_EQ(tenEntityECS.GetHotComponents<TestComponent>()[idx].m_entity, components[idx].m_entity);
		EXPECT_EQ(tenEntityECS.GetHotComponents<TestComponent>()[idx].m_value, components[idx].m_value);

		// Check sparse set.
		EXPECT_EQ(tenEntityECS.GetComponent<TestComponent>(components[idx].m_entity)->m_value, components[idx].m_value);
		EXPECT_EQ(tenEntityECS.GetComponent<TestComponent>(components[idx].m_entity)->m_entity, components[idx].m_entity);
	}

	tenEntityECS.Activate(3);

	components = { TestComponent(0, 10), TestComponent(1, 11), TestComponent(2, 12),
				   TestComponent(9, 19), TestComponent(4, 14), TestComponent(5, 15), 
				   TestComponent(6, 16), TestComponent(7, 17), TestComponent(8, 18),
				   TestComponent(3, 13) };

	for (size_t idx = 0; idx < 10; ++idx)
	{
		EXPECT_EQ(tenEntityECS.GetHotComponents<TestComponent>()[idx].m_entity, components[idx].m_entity);
		EXPECT_EQ(tenEntityECS.GetHotComponents<TestComponent>()[idx].m_value, components[idx].m_value);

		// Check sparse set.
		EXPECT_EQ(tenEntityECS.GetComponent<TestComponent>(components[idx].m_entity)->m_value, components[idx].m_value);
		EXPECT_EQ(tenEntityECS.GetComponent<TestComponent>(components[idx].m_entity)->m_entity, components[idx].m_entity);
	}
}

TEST(ECS, ComponentManagerNotFound)
{
	try
	{
		smallECS.GetHotComponents<TestComponent1>();
	}
	catch (const std::runtime_error& e)
	{
		EXPECT_STREQ(e.what(), "Error: Component Manager not found.");
	}
}

TEST(ECS, EntityActivation)
{
	for (size_t i = 0; i < 1000; ++i)
	{
		mediumECS.AddComponent<TestComponent>(i, i);
	}

	EXPECT_EQ(mediumECS.GetNumEntities(), 1000);
	EXPECT_EQ(mediumECS.GetNumActiveComponents<TestComponent>(), 0);

	std::vector<size_t> activeEntities = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	mediumECS.ActivateEntities(activeEntities);

	const std::vector<TestComponent>& refHotComponents = mediumECS.GetHotComponents<TestComponent>();

	// Should be in order thus far.
	for (size_t i = 0; i < activeEntities.size(); ++i)
	{
		EXPECT_EQ(refHotComponents[i].m_entity, activeEntities[i]);

		// Check sparse set.
		EXPECT_EQ(mediumECS.GetComponent<TestComponent>(activeEntities[i])->m_value, activeEntities[i]);
		EXPECT_EQ(mediumECS.GetComponent<TestComponent>(activeEntities[i])->m_entity, activeEntities[i]);
	}

	mediumECS.DeactivateEntities();

	EXPECT_EQ(mediumECS.GetNumActiveComponents<TestComponent>(), 0);

	for (size_t i = 0; i < activeEntities.size(); ++i)
	{
		EXPECT_FALSE(mediumECS.IsActive<TestComponent>(activeEntities[i]));

		// Check sparse set.
		EXPECT_EQ(mediumECS.GetComponent<TestComponent>(activeEntities[i])->m_value, activeEntities[i]); // Should still work.

	}
}

TEST(MATH, Vector2DCreation)
{
	Engine::Vector2D vec(3.0f, 4.0f);
	EXPECT_FLOAT_EQ(vec.X, 3.0f);
	EXPECT_FLOAT_EQ(vec.Y, 4.0f);
}

TEST(MATH, Vector2DAddition)
{
	Engine::Vector2D vec1(1.0f, 2.0f);
	Engine::Vector2D vec2(3.0f, 4.0f);
	Engine::Vector2D result = vec1 + vec2;
	EXPECT_FLOAT_EQ(result.X, 4.0f);
	EXPECT_FLOAT_EQ(result.Y, 6.0f);
}

TEST(MATH, Vector2DSubtraction)
{
	Engine::Vector2D vec1(5.0f, 7.0f);
	Engine::Vector2D vec2(3.0f, 4.0f);
	Engine::Vector2D result = vec1 - vec2;
	EXPECT_FLOAT_EQ(result.X, 2.0f);
	EXPECT_FLOAT_EQ(result.Y, 3.0f);
}

TEST(MATH, Vector2DScalarMultiplication)
{
	Engine::Vector2D vec(2.0f, 3.0f);
	float scalar = 4.0f;
	Engine::Vector2D result = vec * scalar;
	EXPECT_FLOAT_EQ(result.X, 8.0f);
	EXPECT_FLOAT_EQ(result.Y, 12.0f);
}

TEST(MATH, Vector2DScalarDivision)
{
	Engine::Vector2D vec(8.0f, 12.0f);
	float scalar = 4.0f;
	Engine::Vector2D result = vec / scalar;
	EXPECT_FLOAT_EQ(result.X, 2.0f);
	EXPECT_FLOAT_EQ(result.Y, 3.0f);
}

TEST(MATH, Vector2DCompoundAddition)
{
	Engine::Vector2D vec(1.0f, 2.0f);
	Engine::Vector2D toAdd(3.0f, 4.0f);
	vec += toAdd;
	EXPECT_FLOAT_EQ(vec.X, 4.0f);
	EXPECT_FLOAT_EQ(vec.Y, 6.0f);
}

TEST(MATH, Vector2DCompoundSubtraction)
{
	Engine::Vector2D vec(5.0f, 7.0f);
	Engine::Vector2D toSub(3.0f, 4.0f);
	vec -= toSub;
	EXPECT_FLOAT_EQ(vec.X, 2.0f);
	EXPECT_FLOAT_EQ(vec.Y, 3.0f);
}

TEST(MATH, Vector2DCompoundMultiplication)
{
	Engine::Vector2D vec(2.0f, 3.0f);
	float scalar = 4.0f;
	vec *= scalar;
	EXPECT_FLOAT_EQ(vec.X, 8.0f);
	EXPECT_FLOAT_EQ(vec.Y, 12.0f);
}

TEST(MATH, Vector2DCompoundDivision)
{
	Engine::Vector2D vec(8.0f, 12.0f);
	float scalar = 4.0f;
	vec /= scalar;
	EXPECT_FLOAT_EQ(vec.X, 2.0f);
	EXPECT_FLOAT_EQ(vec.Y, 3.0f);
}



// Entry point for Google Test
int main(int argc, char** argv) 
{
	::testing::InitGoogleTest(&argc, argv);

	// WANT TO SPECIFY TESTS TO RUN? USE:
	return RUN_ALL_TESTS();
}


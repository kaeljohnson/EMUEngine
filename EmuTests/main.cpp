#pragma once

#include <iostream>
#include <chrono>
#include <algorithm>
#include <random>
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

TEST(ECS, TestSparseArrayIteration)
{
	for (size_t i = 0; i < 10000; ++i)
	{
		largeECS.CreateEntity();
		largeECS.AddComponent<TestComponent>(i, i);
		largeECS.Activate(i);
		entitiesForSparseSet.push_back(i);
	}

	EXPECT_EQ(largeECS.GetNumActiveComponents<TestComponent>(), 10000);

	auto start = std::chrono::high_resolution_clock::now();
	for (Engine::Entity& entity : entitiesForSparseSet)
	{
		TestComponent* component = largeECS.GetComponent<TestComponent>(entity);
		EXPECT_EQ(component->m_value, component->m_entity);
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Sparse set iteration: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
}

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

TEST(MATH, Point2DCreation)
{
	Math2D::Point2D vec(3.0f, 4.0f);
	EXPECT_FLOAT_EQ(vec.X, 3.0f);
	EXPECT_FLOAT_EQ(vec.Y, 4.0f);
}

TEST(MATH, Point2DAddition)
{
	Math2D::Point2D vec1(1.0f, 2.0f);
	Math2D::Point2D vec2(3.0f, 4.0f);
	Math2D::Point2D result = vec1 + vec2;
	EXPECT_FLOAT_EQ(result.X, 4.0f);
	EXPECT_FLOAT_EQ(result.Y, 6.0f);
}

TEST(MATH, Point2DSubtraction)
{
	Math2D::Point2D vec1(5.0f, 7.0f);
	Math2D::Point2D vec2(3.0f, 4.0f);
	Math2D::Point2D result = vec1 - vec2;
	EXPECT_FLOAT_EQ(result.X, 2.0f);
	EXPECT_FLOAT_EQ(result.Y, 3.0f);
}

TEST(MATH, Point2DScalarMultiplication)
{
	Math2D::Point2D vec(2.0f, 3.0f);
	float scalar = 4.0f;
	Math2D::Point2D result = vec * scalar;
	EXPECT_FLOAT_EQ(result.X, 8.0f);
	EXPECT_FLOAT_EQ(result.Y, 12.0f);
}

TEST(MATH, Point2DScalarDivision)
{
	Math2D::Point2D vec(8.0f, 12.0f);
	float scalar = 4.0f;
	Math2D::Point2D result = vec / scalar;
	EXPECT_FLOAT_EQ(result.X, 2.0f);
	EXPECT_FLOAT_EQ(result.Y, 3.0f);
}

TEST(MATH, Point2DCompoundAddition)
{
	Math2D::Point2D vec(1.0f, 2.0f);
	Math2D::Point2D toAdd(3.0f, 4.0f);
	vec += toAdd;
	EXPECT_FLOAT_EQ(vec.X, 4.0f);
	EXPECT_FLOAT_EQ(vec.Y, 6.0f);
}

TEST(MATH, Point2DCompoundSubtraction)
{
	Math2D::Point2D vec(5.0f, 7.0f);
	Math2D::Point2D toSub(3.0f, 4.0f);
	vec -= toSub;
	EXPECT_FLOAT_EQ(vec.X, 2.0f);
	EXPECT_FLOAT_EQ(vec.Y, 3.0f);
}

TEST(MATH, Point2DCompoundMultiplication)
{
	Math2D::Point2D vec(2.0f, 3.0f);
	float scalar = 4.0f;
	vec *= scalar;
	EXPECT_FLOAT_EQ(vec.X, 8.0f);
	EXPECT_FLOAT_EQ(vec.Y, 12.0f);
}

TEST(MATH, Point2DCompoundDivision)
{
	Math2D::Point2D vec(8.0f, 12.0f);
	float scalar = 4.0f;
	vec /= scalar;
	EXPECT_FLOAT_EQ(vec.X, 2.0f);
	EXPECT_FLOAT_EQ(vec.Y, 3.0f);
}

TEST(MATH, Lerp)
{
	Math2D::Point2D vec1(0.0f, 0.0f);
	Math2D::Point2D vec2(10.0f, 10.0f);
	float t = 0.5f;
	Math2D::Point2D result = Engine::Lerp(vec1, vec2, t);
	EXPECT_FLOAT_EQ(result.X, 5.0f);
	EXPECT_FLOAT_EQ(result.Y, 5.0f);
}

template <typename T>
void ShuffleVector(std::vector<T>& v)
{
	if (v.size() <= 1) return;  // Never shuffle empty or size-1 vectors

	static thread_local std::mt19937 rng(std::random_device{}());
	std::shuffle(v.begin(), v.end(), rng);
}

static inline bool checkChainEquality(const Math2D::Chain& merged, const Math2D::Chain& expected)
{
	if (merged.m_points.size() != expected.m_points.size() || merged.m_loop != expected.m_loop) return false;

	// Find first common point
	size_t startIndex = SIZE_MAX;
	for (size_t i = 0; i < merged.m_points.size(); ++i)
	{
		if (merged.m_points[i] == expected.m_points[0])
		{
			startIndex = i;
			break;
		}
	}

	if (startIndex == SIZE_MAX)
		return false; // No matching starting point found

	// Check points sequence, which can be rotated.
	for (size_t i = 0; i < expected.m_points.size(); ++i)
	{
		size_t mergedIndex = (startIndex + i) % merged.m_points.size();
		if (!(merged.m_points[mergedIndex] == expected.m_points[i]))
		{
			return false; // Points do not match
		}
	}

	// Check edges sequences, which can be in any order and rotation.
	if (merged.m_originalEdges.size() != expected.m_originalEdges.size())
		return false; // Different number of edges

	std::vector<bool> matched(merged.m_originalEdges.size(), false);
	for (const auto& expectedEdge : expected.m_originalEdges)
	{
		bool foundMatch = false;
		for (size_t j = 0; j < merged.m_originalEdges.size(); ++j)
		{
			if (!matched[j] && merged.m_originalEdges[j] == expectedEdge)
			{
				matched[j] = true;
				foundMatch = true;
				break;
			}
		}

		if (!foundMatch)
		{
			return false; // No matching edge found
		}
	}

	return true;
}

static inline bool checkChainsEquality(const std::vector<Math2D::Chain>& merged, const std::vector<Math2D::Chain>& expected)
{
	EXPECT_EQ(merged.size(), expected.size()) << "Number of merged chains does not match expected number of chains.";

	// For each expected chain, find a matching merged chain
	bool foundMatch = false;
	for (const auto& expectedChain : expected)
	{
		for (const auto& mergedChain : merged)
		{
			if (checkChainEquality(mergedChain, expectedChain))
			{
				foundMatch = true;
				break;
			}
		}

		if (!foundMatch)
		{
			return false;
		}
	}
	return true;
}

TEST(MATH, MergeGridLines)
{
	// Chain 1 edges.
	Engine::Edge edge1 =   { {1.0f, 2.0f}, {1.0f, 1.0f}, {2.0f, 1.0f}, {2.0f, 2.0f} };
	std::cout << "Edge 1 created" << "(" << edge1.m_trailingPoint.X << ", " << edge1.m_trailingPoint.Y << "), "
		<< "(" << edge1.m_startPoint.X << ", " << edge1.m_startPoint.Y << "), "
		<< "(" << edge1.m_endPoint.X << ", " << edge1.m_endPoint.Y << "), "
		<< "(" << edge1.m_leadingPoint.X << ", " << edge1.m_leadingPoint.Y << ")\n";

	Engine::Edge edge2 =  { {1.0f, 1.0f}, {2.0f, 1.0f}, {2.0f, 2.0f}, {3.0f, 2.0f} };
	Engine::Edge edge3 =  { {2.0f, 1.0f}, {2.0f, 2.0f}, {3.0f, 2.0f}, {3.0f, 3.0f} };
	Engine::Edge edge4 =  { {2.0f, 2.0f}, {3.0f, 2.0f}, {3.0f, 3.0f}, {4.0f, 3.0f} };
	Engine::Edge edge5 =  { {3.0f, 2.0f}, {3.0f, 3.0f}, {4.0f, 3.0f}, {4.0f, 4.0f} };
	Engine::Edge edge6 =  { {3.0f, 3.0f}, {4.0f, 3.0f}, {4.0f, 4.0f}, {3.0f, 4.0f} };
	Engine::Edge edge7 =  { {4.0f, 3.0f}, {4.0f, 4.0f}, {3.0f, 4.0f}, {3.0f, 3.0f} };
	Engine::Edge edge8 =  { {4.0f, 4.0f}, {3.0f, 4.0f}, {3.0f, 3.0f}, {2.0f, 3.0f} };
	Engine::Edge edge9 =  { {3.0f, 4.0f}, {3.0f, 3.0f}, {2.0f, 3.0f}, {2.0f, 2.0f} };
	Engine::Edge edge10 = { {3.0f, 3.0f}, {2.0f, 3.0f}, {2.0f, 2.0f}, {1.0f, 2.0f} };
	Engine::Edge edge11 = { {2.0f, 3.0f}, {2.0f, 2.0f}, {1.0f, 2.0f}, {1.0f, 1.0f} };
	Engine::Edge edge12 = { {2.0f, 2.0f}, {1.0f, 2.0f}, {1.0f, 1.0f}, {2.0f, 1.0f} };
	

	// Chain 2 edges.
	Engine::Edge edge13 = { {5.0f, 2.0f}, {5.0f, 1.0f}, {6.0f, 1.0f}, {6.0f, 2.0f} };
	Engine::Edge edge14 = { {5.0f, 1.0f}, {6.0f, 1.0f}, {6.0f, 2.0f}, {7.0f, 2.0f} };
	Engine::Edge edge15 = { {6.0f, 1.0f}, {6.0f, 2.0f}, {7.0f, 2.0f}, {7.0f, 3.0f} };
	Engine::Edge edge16 = { {6.0f, 2.0f}, {7.0f, 2.0f}, {7.0f, 3.0f}, {6.0f, 3.0f} };
	Engine::Edge edge17 = { {7.0f, 2.0f}, {7.0f, 3.0f}, {6.0f, 3.0f}, {5.0f, 3.0f} };
	Engine::Edge edge18 = { {7.0f, 3.0f}, {6.0f, 3.0f}, {5.0f, 3.0f}, {5.0f, 2.0f} };
	Engine::Edge edge19 = { {6.0f, 3.0f}, {5.0f, 3.0f}, {5.0f, 2.0f}, {5.0f, 1.0f} };
	Engine::Edge edge20 = { {5.0f, 3.0f}, {5.0f, 2.0f}, {5.0f, 1.0f}, {6.0f, 1.0f} };

	// Chain 3 edges
	Engine::Edge edge21 = { {1.0f, 6.0f}, {1.0f, 5.0f}, {2.0f, 5.0f}, {3.0f, 5.0f} };
	Engine::Edge edge22 = { {1.0f, 5.0f}, {2.0f, 5.0f}, {3.0f, 5.0f}, {3.0f, 6.0f} };
	Engine::Edge edge23 = { {2.0f, 5.0f}, {3.0f, 5.0f}, {3.0f, 6.0f}, {2.0f, 6.0f} };
	Engine::Edge edge24 = { {3.0f, 5.0f}, {3.0f, 6.0f}, {2.0f, 6.0f}, {1.0f, 6.0f} };
	Engine::Edge edge25 = { {3.0f, 6.0f}, {2.0f, 6.0f}, {1.0f, 6.0f}, {1.0f, 5.0f} };
	Engine::Edge edge26 = { {2.0f, 6.0f}, {1.0f, 6.0f}, {1.0f, 5.0f}, {2.0f, 5.0f} };

	// Chain 4 edges
	Engine::Edge edge27 = { {5.0f, 5.0f}, {5.0f, 4.0f}, {6.0f, 4.0f}, {7.0f, 4.0f} };
	Engine::Edge edge28 = { {5.0f, 4.0f}, {6.0f, 4.0f}, {7.0f, 4.0f}, {8.0f, 4.0f} };
	Engine::Edge edge29 = { {6.0f, 4.0f}, {7.0f, 4.0f}, {8.0f, 4.0f}, {8.0f, 5.0f} };
	Engine::Edge edge30 = { {7.0f, 4.0f}, {8.0f, 4.0f}, {8.0f, 5.0f}, {8.0f, 6.0f} };
	Engine::Edge edge31 = { {8.0f, 4.0f}, {8.0f, 5.0f}, {8.0f, 6.0f}, {8.0f, 7.0f} };
	Engine::Edge edge32 = { {8.0f, 5.0f}, {8.0f, 6.0f}, {8.0f, 7.0f}, {7.0f, 7.0f} };
	Engine::Edge edge33 = { {8.0f, 6.0f}, {8.0f, 7.0f}, {7.0f, 7.0f}, {6.0f, 7.0f} };
	Engine::Edge edge34 = { {8.0f, 7.0f}, {7.0f, 7.0f}, {6.0f, 7.0f}, {5.0f, 7.0f} };
	Engine::Edge edge35 = { {7.0f, 7.0f}, {6.0f, 7.0f}, {5.0f, 7.0f}, {5.0f, 6.0f} };
	Engine::Edge edge36 = { {6.0f, 7.0f}, {5.0f, 7.0f}, {5.0f, 6.0f}, {5.0f, 5.0f} };
	Engine::Edge edge37 = { {5.0f, 7.0f}, {5.0f, 6.0f}, {5.0f, 5.0f}, {5.0f, 4.0f} };
	Engine::Edge edge38 = { {5.0f, 6.0f}, {5.0f, 5.0f}, {5.0f, 4.0f}, {6.0f, 4.0f} };

	// Chain 5 edges
	Engine::Edge edge39 = { {6.0f, 6.0f}, {6.0f, 5.0f}, {7.0f, 5.0f}, {7.0f, 6.0f} };
	Engine::Edge edge40 = { {6.0f, 5.0f}, {7.0f, 5.0f}, {7.0f, 6.0f}, {6.0f, 6.0f} };
	Engine::Edge edge41 = { {7.0f, 5.0f}, {7.0f, 6.0f}, {6.0f, 6.0f}, {6.0f, 5.0f} };
	Engine::Edge edge42 = { {7.0f, 6.0f}, {6.0f, 6.0f}, {6.0f, 5.0f}, {7.0f, 5.0f} };

	// Chain 6 edges (not forming a loop, should remain separate)
	Engine::Edge edge43 = { {10.0f, 10.0f}, {11.0f, 10.0f}, {12.0f, 10.0f}, {13.0f, 10.0f} };
	Engine::Edge edge44 = { {11.0f, 10.0f}, {12.0f, 10.0f}, {13.0f, 10.0f}, {14.0f, 10.0f} };
	Engine::Edge edge45 = { {12.0f, 10.0f}, {13.0f, 10.0f}, {14.0f, 10.0f}, {15.0f, 10.0f} };
	Engine::Edge edge46 = { {13.0f, 10.0f}, {14.0f, 10.0f}, {15.0f, 10.0f}, {16.0f, 10.0f} };
	Engine::Edge edge47 = { {14.0f, 10.0f}, {15.0f, 10.0f}, {16.0f, 10.0f}, {17.0f, 10.0f} };
	Engine::Edge edge48 = { {15.0f, 10.0f}, {16.0f, 10.0f}, {17.0f, 10.0f}, {18.0f, 10.0f} };

	std::vector<Engine::Edge> edges =
	{ edge1, edge2, edge3, edge4, edge5, edge6, edge7, edge8, edge9, edge10, edge11, edge12,
	  edge13, edge14, edge15, edge16, edge17, edge18, edge19, edge20,
	  edge21, edge22, edge23, edge24, edge25, edge26,
	  edge27, edge28, edge29, edge30, edge31, edge32, edge33, edge34, edge35, edge36, edge37, edge38,
	  edge39, edge40, edge41, edge42,
	  edge43, edge44, edge45, edge46, edge47, edge48
	};

	// Create a random device and a generator
	std::random_device rd;
	std::mt19937 g(rd());

	//// Shuffle the vector
	ShuffleVector(edges); // Merge function should be able to handle an edges vector in any order.

	std::vector<Math2D::Chain> mergedChains = Engine::MergeGridLinesIntoChains(edges);

	for (const auto& chain : mergedChains)
	{
		std::cout << "Merged Chain Points: ";
		for (const auto& point : chain.m_points)
		{
			std::cout << "(" << point.X << ", " << point.Y << ") ";
		}
		std::cout << " : " << chain.m_loop << "\n";
	}

	Math2D::Chain expectedChain1 = 
	{
		{ {1.0f, 1.0f}, {2.0f, 1.0f}, {2.0f, 2.0f}, {3.0f, 2.0f},
		  {3.0f, 3.0f}, {4.0f, 3.0f}, {4.0f, 4.0f}, {3.0f, 4.0f},
		  {3.0f, 3.0f}, {2.0f, 3.0f}, {2.0f, 2.0f}, {1.0f, 2.0f} },
		{ edge1, edge2, edge3, edge4, edge5, edge6, edge7, edge8, edge9, edge10, edge11, edge12 },
		true
	};

	Math2D::Chain expectedChain2 = 
	{
		{ {5.0f, 1.0f}, {6.0f, 1.0f}, {6.0f, 2.0f}, {7.0f, 2.0f},
		  {7.0f, 3.0f}, {6.0f, 3.0f}, {5.0f, 3.0f}, {5.0f, 2.0f} },
		{ edge13, edge14, edge15, edge16, edge17, edge18, edge19, edge20 },
		true
	};

	Math2D::Chain expectedChain3 = 
	{
		{ {1.0f, 5.0f}, {2.0f, 5.0f}, {3.0f, 5.0f}, {3.0f, 6.0f},
		  {2.0f, 6.0f}, {1.0f, 6.0f} },
		{ edge21, edge22, edge23, edge24, edge25, edge26 },
		true
	};

	Math2D::Chain expectedChain4 = 
	{
		{ {5.0f, 4.0f}, {6.0f, 4.0f}, {7.0f, 4.0f}, {8.0f, 4.0f},
		  {8.0f, 5.0f}, {8.0f, 6.0f}, {8.0f, 7.0f}, {7.0f, 7.0f},
		  {6.0f, 7.0f}, {5.0f, 7.0f}, {5.0f, 6.0f}, {5.0f, 5.0f} },
		{ edge27, edge28, edge29, edge30, edge31, edge32, edge33,
		  edge34, edge35, edge36, edge37, edge38 },
		true
	};

	Math2D::Chain expectedChain5 = 
	{
		{ {6.0f, 5.0f}, {7.0f, 5.0f}, {7.0f, 6.0f}, {6.0f, 6.0f} },
		{ edge39, edge40, edge41, edge42 },
		true
	};

	Math2D::Chain expectedChain6 =
	{
		{ {11.0f, 10.0f}, {12.0f, 10.0f}, {13.0f, 10.0f},
		  {14.0f, 10.0f}, {15.0f, 10.0f}, {16.0f, 10.0f}, {17.0f, 10.0f} },
		{ edge43, edge44, edge45, edge46, edge47, edge48 },
		false
	};

	std::vector<Math2D::Chain> expectedChains = { expectedChain1, expectedChain2, expectedChain3, expectedChain4, expectedChain5, expectedChain6 };

	for (const auto& chain : expectedChains)
	{
		std::cout << "Expected Chain Points: ";
		for (const auto& point : chain.m_points)
		{
			std::cout << "(" << point.X << ", " << point.Y << ") ";
		}
		std::cout << "\n";
	}

	EXPECT_TRUE(checkChainsEquality(mergedChains, expectedChains));
};

// Entry point for Google Test
int main(int argc, char** argv) 
{
	::testing::InitGoogleTest(&argc, argv);

	// WANT TO SPECIFY TESTS TO RUN? USE:
	return RUN_ALL_TESTS();
}


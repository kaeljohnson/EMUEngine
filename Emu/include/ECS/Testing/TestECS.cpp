#pragma once

#include <iostream>
#include <chrono>
#include "../ECS.h"

using namespace Engine;


struct TestComponent 
{
    int value;
};

void RunTests() 
{
    ECS ecs;
    ecs.Initialize(1000);

    /* Basic Tests */

    // Test entity creation
    auto start = std::chrono::high_resolution_clock::now();
    Entity* entity = ecs.CreateEntity();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Entity creation time: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";

    // Test component management
    ecs.RegisterComponentManager<TestComponent>();
    start = std::chrono::high_resolution_clock::now();
    ecs.AddComponent<TestComponent>(entity, TestComponent{ 42 });
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Component addition time: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
}

int main() 
{
    RunTests();
    return 0;
}


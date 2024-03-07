#pragma once

#include <stdio.h>

#include "../include/EngineIncludes.h"

const char* APPLICATION_NAME = "Wizard Game v1";\

// Main function. Starting point for all C++ programs.
int main(int argc, char* args[])
{
	// Call to Init function. Initializes the logger, the SDL2 and SDL2_image libraries.
	Engine::Init();
	// Call to Game constructor. Creates or "instantiates" and object of type "Game" named game.
	Engine::Application app(APPLICATION_NAME);
	// Log a warning if app was instantiated without initializing the engine.

	CLIENT_TRACE("Client Running!");

	Engine::GameObject go(20, 20, true, true);
	Engine::Layer testLayer("Test Layer 1");
	Engine::Layer testLayer2("Test Layer 2");
	Engine::Layer testLayer3("Test Layer 3");

	CLIENT_TRACE("STOP 1");
	app.addToLayerStack(&testLayer); 
	app.addToLayerStack(&testLayer2);
	app.addToLayerStack(&testLayer3);
	app.popLayerFromStack(testLayer2.getName());

	for (Engine::Layer* layer : app.getLayerStack()) 
	{
		CLIENT_TRACE("Layer: {0}", layer->getName());
	}

	app.popLayerFromStack();
	
	for (Engine::Layer* layer : app.getLayerStack())
	{
		CLIENT_TRACE("Layer: {0}", layer->getName());
	}

	app.popLayerFromStack();
	CLIENT_TRACE("STOP 2");
	
	app.run();

	return 0;
}
#pragma once

#include <stdio.h>
#include <unordered_map>
#include "../Events/Event.h"

namespace Engine
{
	void handleQuitEvent(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle quit event.\n"); }

	void handleWindowResize(std::unordered_map<EventType, Event>& refEventQ, const int data1, const int data2) { printf("Default handle resize event."); }

	void handleEqualsDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle equals down event."); }
	void handleEqualsUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle equals up event."); }

	void handleEscapeKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle esc key down.\n"); }
	void handleEscapeKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle esc key up.\n"); }

	void handleZeroKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle zero key down.\n"); }
	void handleZeroKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle zero key up.\n"); }

	void handleOneKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle one key down.\n"); }
	void handleOneKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle one key up.\n"); }

	void handleTwoKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle two key down.\n"); }
	void handleTwoKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle two key up.\n"); }

	void handleThreeKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle three key down.\n"); }
	void handleThreeKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle three key up.\n"); }

	void handleFourKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle four key down.\n"); }
	void handleFourKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle four key up.\n"); }

	void handleFiveKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle five key down.\n"); }
	void handleFiveKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle five key up.\n"); }

	void handleSixKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle six key down.\n"); }
	void handleSixKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle six key up.\n"); }

	void handleSevenKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle seven key down.\n"); }
	void handleSevenKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle seven key up.\n"); }

	void handleEightKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle eight key down.\n"); }
	void handleEightKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle eight key up.\n"); }

	void handleNineKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle nine key down.\n"); }
	void handleNineKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle nine key up.\n"); }

	void handleQKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle Q key down.\n"); }
	void handleQKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle Q key up.\n"); }

	void handleWKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle W key down.\n"); }
	void handleWKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle W key up.\n"); }

	void handleEKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle E key down.\n"); }
	void handleEKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle E key up.\n"); }

	void handleRKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle R key down.\n"); }
	void handleRKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle R key up.\n"); }

	void handleTKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle T key down.\n"); }
	void handleTKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle T key up.\n"); }

	void handleYKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle Y key down.\n"); }
	void handleYKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle Y key up.\n"); }

	void handleUKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle U key down.\n"); }
	void handleUKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle U key up.\n"); }

	void handleIKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle I key down.\n"); }
	void handleIKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle I key up.\n"); }

	void handleOKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle O key down.\n"); }
	void handleOKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle O key up.\n"); }

	void handlePKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle P key down.\n"); }
	void handlePKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle P key up.\n"); }

	void handleAKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle A key down.\n"); }
	void handleAKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle A key up.\n"); }

	void handleSKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle S key down.\n"); }
	void handleSKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle S key up.\n"); }

	void handleDKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle D key down.\n"); }
	void handleDKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle D key up.\n"); }

	void handleFKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle F key down.\n"); }
	void handleFKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle F key up.\n"); }

	void handleGKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle G key down.\n"); }
	void handleGKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle G key up.\n"); }

	void handleHKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle H key down.\n"); }
	void handleHKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle H key up.\n"); }

	void handleJKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle J key down.\n"); }
	void handleJKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle J key up.\n"); }

	void handleKKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle K key down.\n"); }
	void handleKKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle K key up.\n"); }

	void handleLKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle L key down.\n"); }
	void handleLKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle L key up.\n"); }

	void handleZKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle Z key down.\n"); }
	void handleZKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle Z key up.\n"); }

	void handleXKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle X key down.\n"); }
	void handleXKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle X key up.\n"); }

	void handleCKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle C key down.\n"); }
	void handleCKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle C key up.\n"); }

	void handleVKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle V key down.\n"); }
	void handleVKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle V key up.\n"); }

	void handleBKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle B key down.\n"); }
	void handleBKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle B key up.\n"); }

	void handleNKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle N key down.\n"); }
	void handleNKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle N key up.\n"); }

	void handleMKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle M key down.\n"); }
	void handleMKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle M key up.\n"); }

	void handleLeftArrowKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle left arrow key down.\n"); }
	void handleLeftArrowKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle left arrow key up.\n"); }

	void handleUpArrowKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle up arrow key down.\n"); }
	void handleUpArrowKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle up arrow key up.\n"); }

	void handleDownArrowKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle down arrow key down.\n"); }
	void handleDownArrowKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle down arrow key up.\n"); }

	void handleRightArrowKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle right arrow key down.\n"); }
	void handleRightArrowKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle right arrow key up.\n"); }


	void handleSpaceKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle space key down.\n"); }
	void handleSpaceKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle space key up.\n"); }

	void handleTabKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle tab key down.\n"); }
	void handleTabKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle tab key up.\n"); }

	void handleCapsLockKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle caps lock down.\n"); }
	void handleCapsLockKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle caps lock up.\n"); }

	void handleLShiftKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle lShift key down.\n"); }
	void handleLShiftKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle lShift key up.\n"); }

	void handleEnterKeyDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle enter key down.\n"); }
	void handleEnterKeyUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle enter key up.\n"); }


	void handleMouseLeftDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle left mouse down.\n"); }
	void handleMouseLeftUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle left mouse up.\n"); }

	void handleMouseMiddleDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle middle mouse down.\n"); }
	void handleMouseMiddleUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle midde mouse up.\n"); }

	void handleMouseRightDown(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle right mouse down.\n"); }
	void handleMouseRightUp(std::unordered_map<EventType, Event>& refEventQ) { printf("Default handle right mouse up.\n"); }

	void handleMouseScroll(std::unordered_map<EventType, Event>& refEventQ, int xDir, int yDir) { printf("Default handle mouse scroll. Direction: X: %d, Y: %d\n", xDir, yDir); }

	void handleMouseMove(std::unordered_map<EventType, Event>& refEventQ, int xPos, int yPos) { printf("Default handle mouse move. Position: X: %d, Y: %d\n", xPos, yPos); }
}
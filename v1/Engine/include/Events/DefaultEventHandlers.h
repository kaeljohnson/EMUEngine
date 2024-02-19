#pragma once

#include <stdio.h>
#include <unordered_map>
#include "../Events/Event.h"
#include "../Logging/Logger.h"

namespace Engine
{
	void handleQuitEvent(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle quit event."); }

	void handleWindowResize(std::unordered_map<EventType, Event>& refEventQ, const int data1, const int data2) { ENGINE_TRACE("Default handle resize event."); }

	void handleEqualsDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle equals down event."); }
	void handleEqualsUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle equals up event."); }

	void handleEscapeKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle esc key down."); }
	void handleEscapeKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle esc key up."); }

	void handleZeroKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle zero key down."); }
	void handleZeroKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle zero key up."); }

	void handleOneKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle one key down."); }
	void handleOneKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle one key up."); }

	void handleTwoKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle two key down."); }
	void handleTwoKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle two key up."); }

	void handleThreeKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle three key down."); }
	void handleThreeKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle three key up."); }

	void handleFourKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle four key down."); }
	void handleFourKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle four key up."); }

	void handleFiveKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle five key down."); }
	void handleFiveKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle five key up."); }

	void handleSixKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle six key down."); }
	void handleSixKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle six key up."); }

	void handleSevenKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle seven key down."); }
	void handleSevenKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle seven key up."); }

	void handleEightKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle eight key down."); }
	void handleEightKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle eight key up."); }

	void handleNineKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle nine key down."); }
	void handleNineKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle nine key up."); }

	void handleQKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle Q key down."); }
	void handleQKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle Q key up."); }

	void handleWKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle W key down."); }
	void handleWKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle W key up."); }

	void handleEKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle E key down."); }
	void handleEKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle E key up."); }

	void handleRKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle R key down."); }
	void handleRKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle R key up."); }

	void handleTKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle T key down."); }
	void handleTKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle T key up."); }

	void handleYKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle Y key down."); }
	void handleYKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle Y key up."); }

	void handleUKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle U key down."); }
	void handleUKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle U key up."); }

	void handleIKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle I key down."); }
	void handleIKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle I key up."); }

	void handleOKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle O key down."); }
	void handleOKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle O key up."); }

	void handlePKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle P key down."); }
	void handlePKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle P key up."); }

	void handleAKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle A key down."); }
	void handleAKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle A key up."); }

	void handleSKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle S key down."); }
	void handleSKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle S key up."); }

	void handleDKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle D key down."); }
	void handleDKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle D key up."); }

	void handleFKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle F key down."); }
	void handleFKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle F key up."); }

	void handleGKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle G key down."); }
	void handleGKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle G key up."); }

	void handleHKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle H key down."); }
	void handleHKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle H key up."); }

	void handleJKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle J key down."); }
	void handleJKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle J key up."); }

	void handleKKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle K key down."); }
	void handleKKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle K key up."); }

	void handleLKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle L key down."); }
	void handleLKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle L key up."); }

	void handleZKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle Z key down."); }
	void handleZKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle Z key up."); }

	void handleXKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle X key down."); }
	void handleXKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle X key up."); }

	void handleCKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle C key down."); }
	void handleCKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle C key up."); }

	void handleVKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle V key down."); }
	void handleVKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle V key up."); }

	void handleBKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle B key down."); }
	void handleBKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle B key up."); }

	void handleNKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle N key down."); }
	void handleNKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle N key up."); }

	void handleMKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle M key down."); }
	void handleMKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle M key up."); }

	void handleLeftArrowKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle left arrow key down."); }
	void handleLeftArrowKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle left arrow key up."); }

	void handleUpArrowKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle up arrow key down."); }
	void handleUpArrowKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle up arrow key up."); }

	void handleDownArrowKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle down arrow key down."); }
	void handleDownArrowKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle down arrow key up."); }

	void handleRightArrowKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle right arrow key down."); }
	void handleRightArrowKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle right arrow key up."); }


	void handleSpaceKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle space key down."); }
	void handleSpaceKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle space key up."); }

	void handleTabKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle tab key down."); }
	void handleTabKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle tab key up."); }

	void handleCapsLockKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle caps lock down."); }
	void handleCapsLockKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle caps lock up."); }

	void handleLShiftKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle lShift key down."); }
	void handleLShiftKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle lShift key up."); }

	void handleEnterKeyDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle enter key down."); }
	void handleEnterKeyUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle enter key up."); }


	void handleMouseLeftDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle left mouse down."); }
	void handleMouseLeftUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle left mouse up."); }

	void handleMouseMiddleDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle middle mouse down."); }
	void handleMouseMiddleUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle midde mouse up."); }

	void handleMouseRightDown(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle right mouse down."); }
	void handleMouseRightUp(std::unordered_map<EventType, Event>& refEventQ) { ENGINE_TRACE("Default handle right mouse up."); }

	void handleMouseScroll(std::unordered_map<EventType, Event>& refEventQ, int xDir, int yDir) { ENGINE_TRACE("Default handle mouse scroll. Direction: X: %d, Y: %d", xDir, yDir); }

	void handleMouseMove(std::unordered_map<EventType, Event>& refEventQ, int xPos, int yPos) { ENGINE_TRACE("Default handle mouse move. Position: X: %d, Y: %d", xPos, yPos); }
}
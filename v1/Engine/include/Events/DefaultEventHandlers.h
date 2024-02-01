#pragma once

#include <stdio.h>
#include <queue>
#include "../Actions/ActionsEnum.h"

namespace Engine
{
	void handleQuitEvent(std::queue<Actions>& refActionsQ) { printf("Default handle quit event.\n"); refActionsQ.push(QUIT); }

	void handleWindowResize(std::queue<Actions>& refActionsQ) { printf("Default handle resize event."); refActionsQ.push(RESIZE_WINDOW); }

	void handleEqualsDown(std::queue<Actions>& refActionsQ) { printf("Default handle equals down event."); refActionsQ.push(TOGGLE_FULLSCREEN); }
	void handleEqualsUp() { printf("Default handle equals up event."); }

	void handleEscapeKeyDown() { printf("Default handle esc key down.\n"); }
	void handleEscapeKeyUp() { printf("Default handle esc key up.\n"); }

	void handleZeroKeyDown() { printf("Default handle zero key down.\n"); }
	void handleZeroKeyUp() { printf("Default handle zero key up.\n"); }

	void handleOneKeyDown() { printf("Default handle one key down.\n"); }
	void handleOneKeyUp() { printf("Default handle one key up.\n"); }

	void handleTwoKeyDown() { printf("Default handle two key down.\n"); }
	void handleTwoKeyUp() { printf("Default handle two key up.\n"); }

	void handleThreeKeyDown() { printf("Default handle three key down.\n"); }
	void handleThreeKeyUp() { printf("Default handle three key up.\n"); }

	void handleFourKeyDown() { printf("Default handle four key down.\n"); }
	void handleFourKeyUp() { printf("Default handle four key up.\n"); }

	void handleFiveKeyDown() { printf("Default handle five key down.\n"); }
	void handleFiveKeyUp() { printf("Default handle five key up.\n"); }

	void handleSixKeyDown() { printf("Default handle six key down.\n"); }
	void handleSixKeyUp() { printf("Default handle six key up.\n"); }

	void handleSevenKeyDown() { printf("Default handle seven key down.\n"); }
	void handleSevenKeyUp() { printf("Default handle seven key up.\n"); }

	void handleEightKeyDown() { printf("Default handle eight key down.\n"); }
	void handleEightKeyUp() { printf("Default handle eight key up.\n"); }

	void handleNineKeyDown() { printf("Default handle nine key down.\n"); }
	void handleNineKeyUp() { printf("Default handle nine key up.\n"); }

	void handleQKeyDown() { printf("Default handle Q key down.\n"); }
	void handleQKeyUp() { printf("Default handle Q key up.\n"); }

	void handleWKeyDown() { printf("Default handle W key down.\n"); }
	void handleWKeyUp() { printf("Default handle W key up.\n"); }

	void handleEKeyDown() { printf("Default handle E key down.\n"); }
	void handleEKeyUp() { printf("Default handle E key up.\n"); }

	void handleRKeyDown() { printf("Default handle R key down.\n"); }
	void handleRKeyUp() { printf("Default handle R key up.\n"); }

	void handleTKeyDown() { printf("Default handle T key down.\n"); }
	void handleTKeyUp() { printf("Default handle T key up.\n"); }

	void handleYKeyDown() { printf("Default handle Y key down.\n"); }
	void handleYKeyUp() { printf("Default handle Y key up.\n"); }

	void handleUKeyDown() { printf("Default handle U key down.\n"); }
	void handleUKeyUp() { printf("Default handle U key up.\n"); }

	void handleIKeyDown() { printf("Default handle I key down.\n"); }
	void handleIKeyUp() { printf("Default handle I key up.\n"); }

	void handleOKeyDown() { printf("Default handle O key down.\n"); }
	void handleOKeyUp() { printf("Default handle O key up.\n"); }

	void handlePKeyDown() { printf("Default handle P key down.\n"); }
	void handlePKeyUp() { printf("Default handle P key up.\n"); }

	void handleAKeyDown() { printf("Default handle A key down.\n"); }
	void handleAKeyUp() { printf("Default handle A key up.\n"); }

	void handleSKeyDown() { printf("Default handle S key down.\n"); }
	void handleSKeyUp() { printf("Default handle S key up.\n"); }

	void handleDKeyDown() { printf("Default handle D key down.\n"); }
	void handleDKeyUp() { printf("Default handle D key up.\n"); }

	void handleFKeyDown() { printf("Default handle F key down.\n"); }
	void handleFKeyUp() { printf("Default handle F key up.\n"); }

	void handleGKeyDown() { printf("Default handle G key down.\n"); }
	void handleGKeyUp() { printf("Default handle G key up.\n"); }

	void handleHKeyDown() { printf("Default handle H key down.\n"); }
	void handleHKeyUp() { printf("Default handle H key up.\n"); }

	void handleJKeyDown() { printf("Default handle J key down.\n"); }
	void handleJKeyUp() { printf("Default handle J key up.\n"); }

	void handleKKeyDown() { printf("Default handle K key down.\n"); }
	void handleKKeyUp() { printf("Default handle K key up.\n"); }

	void handleLKeyDown() { printf("Default handle L key down.\n"); }
	void handleLKeyUp() { printf("Default handle L key up.\n"); }

	void handleZKeyDown() { printf("Default handle Z key down.\n"); }
	void handleZKeyUp() { printf("Default handle Z key up.\n"); }

	void handleXKeyDown() { printf("Default handle X key down.\n"); }
	void handleXKeyUp() { printf("Default handle X key up.\n"); }

	void handleCKeyDown() { printf("Default handle C key down.\n"); }
	void handleCKeyUp() { printf("Default handle C key up.\n"); }

	void handleVKeyDown() { printf("Default handle V key down.\n"); }
	void handleVKeyUp() { printf("Default handle V key up.\n"); }

	void handleBKeyDown() { printf("Default handle B key down.\n"); }
	void handleBKeyUp() { printf("Default handle B key up.\n"); }

	void handleNKeyDown() { printf("Default handle N key down.\n"); }
	void handleNKeyUp() { printf("Default handle N key up.\n"); }

	void handleMKeyDown() { printf("Default handle M key down.\n"); }
	void handleMKeyUp() { printf("Default handle M key up.\n"); }

	void handleLeftArrowKeyDown() { printf("Default handle left arrow key down.\n"); }
	void handleLeftArrowKeyUp() { printf("Default handle left arrow key up.\n"); }

	void handleUpArrowKeyDown() { printf("Default handle up arrow key down.\n"); }
	void handleUpArrowKeyUp() { printf("Default handle up arrow key up.\n"); }

	void handleDownArrowKeyDown() { printf("Default handle down arrow key down.\n"); }
	void handleDownArrowKeyUp() { printf("Default handle down arrow key up.\n"); }

	void handleRightArrowKeyDown() { printf("Default handle right arrow key down.\n"); }
	void handleRightArrowKeyUp() { printf("Default handle right arrow key up.\n"); }


	void handleSpaceKeyDown() { printf("Default handle space key down.\n"); }
	void handleSpaceKeyUp() { printf("Default handle space key up.\n"); }

	void handleTabKeyDown() { printf("Default handle tab key down.\n"); }
	void handleTabKeyUp() { printf("Default handle tab key up.\n"); }

	void handleCapsLockKeyDown() { printf("Default handle caps lock down.\n"); }
	void handleCapsLockKeyUp() { printf("Default handle caps lock up.\n"); }

	void handleLShiftKeyDown() { printf("Default handle lShift key down.\n"); }
	void handleLShiftKeyUp() { printf("Default handle lShift key up.\n"); }

	void handleEnterKeyDown() { printf("Default handle enter key down.\n"); }
	void handleEnterKeyUp() { printf("Default handle enter key up.\n"); }


	void handleMouseLeftDown() { printf("Default handle left mouse down.\n"); }
	void handleMouseLeftUp() { printf("Default handle left mouse up.\n"); }

	void handleMouseMiddleDown() { printf("Default handle middle mouse down.\n"); }
	void handleMouseMiddleUp() { printf("Default handle midde mouse up.\n"); }

	void handleMouseRightDown() { printf("Default handle right mouse down.\n"); }
	void handleMouseRightUp() { printf("Default handle right mouse up.\n"); }

	void handleMouseScroll(int xDir, int yDir) { printf("Default handle mouse scroll. Direction: X: %d, Y: %d\n", xDir, yDir); }

	void handleMouseMove(int xPos, int yPos) { printf("Default handle mouse move. Position: X: %d, Y: %d\n", xPos, yPos); }
}
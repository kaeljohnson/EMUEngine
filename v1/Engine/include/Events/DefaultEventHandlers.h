#pragma once

#include <stdio.h>
#include <queue>
#include "../Actions/ActionsEnum.h"

namespace Engine
{
	void handleQuitEvent(std::queue<Actions>& refActionsQ) { printf("Default handle quit event.\n"); refActionsQ.push(QUIT); }

	void handleWindowResize(std::queue<Actions>& refActionsQ) { printf("Default handle resize event."); refActionsQ.push(RESIZE_WINDOW); }

	void handleEqualsDown(std::queue<Actions>& refActionsQ) { printf("Default handle equals down event."); refActionsQ.push(TOGGLE_FULLSCREEN); }
	void handleEqualsHeld() { printf("Default handle equals held event."); }
	void handleEqualsUp() { printf("Default handle equals up event."); }

	void handleEscapeKeyDown() { printf("Default handle esc key down.\n"); }
	void handleEscapeKeyHeld() { printf("Default handle esc key held.\n"); }
	void handleEscapeKeyUp() { printf("Default handle esc key up.\n"); }

	void handleZeroKeyDown() { printf("Default handle zero key down.\n"); }
	void handleZeroKeyHeld() { printf("Default handle zero key held.\n"); }
	void handleZeroKeyUp() { printf("Default handle zero key up.\n"); }

	void handleOneKeyDown() { printf("Default handle one key down.\n"); }
	void handleOneKeyHeld() { printf("Default handle one key held.\n"); }
	void handleOneKeyUp() { printf("Default handle one key up.\n"); }

	void handleTwoKeyDown() { printf("Default handle two key down.\n"); }
	void handleTwoKeyHeld() { printf("Default handle two key held.\n"); }
	void handleTwoKeyUp() { printf("Default handle two key up.\n"); }

	void handleThreeKeyDown() { printf("Default handle three key down.\n"); }
	void handleThreeKeyHeld() { printf("Default handle three key held.\n"); }
	void handleThreeKeyUp() { printf("Default handle three key up.\n"); }

	void handleFourKeyDown() { printf("Default handle four key down.\n"); }
	void handleFourKeyHeld() { printf("Default handle four key held.\n"); }
	void handleFourKeyUp() { printf("Default handle four key up.\n"); }

	void handleFiveKeyDown() { printf("Default handle five key down.\n"); }
	void handleFiveKeyHeld() { printf("Default handle five key held.\n"); }
	void handleFiveKeyUp() { printf("Default handle five key up.\n"); }

	void handleSixKeyDown() { printf("Default handle six key down.\n"); }
	void handleSixKeyHeld() { printf("Default handle six key held.\n"); }
	void handleSixKeyUp() { printf("Default handle six key up.\n"); }

	void handleSevenKeyDown() { printf("Default handle seven key down.\n"); }
	void handleSevenKeyHeld() { printf("Default handle seven key held.\n"); }
	void handleSevenKeyUp() { printf("Default handle seven key up.\n"); }

	void handleEightKeyDown() { printf("Default handle eight key down.\n"); }
	void handleEightKeyHeld() { printf("Default handle eight key held.\n"); }
	void handleEightKeyUp() { printf("Default handle eight key up.\n"); }

	void handleNineKeyDown() { printf("Default handle nine key down.\n"); }
	void handleNineKeyHeld() { printf("Default handle nine key held.\n"); }
	void handleNineKeyUp() { printf("Default handle nine key up.\n"); }

	void handleQKeyDown() { printf("Default handle Q key down.\n"); }
	void handleQKeyHeld() { printf("Default handle Q key held.\n"); }
	void handleQKeyUp() { printf("Default handle Q key up.\n"); }

	void handleWKeyDown() { printf("Default handle W key down.\n"); }
	void handleWKeyHeld() { printf("Default handle W key held.\n"); }
	void handleWKeyUp() { printf("Default handle W key up.\n"); }

	void handleEKeyDown() { printf("Default handle E key down.\n"); }
	void handleEKeyHeld() { printf("Default handle E key held.\n"); }
	void handleEKeyUp() { printf("Default handle E key up.\n"); }

	void handleRKeyDown() { printf("Default handle R key down.\n"); }
	void handleRKeyHeld() { printf("Default handle R key held.\n"); }
	void handleRKeyUp() { printf("Default handle R key up.\n"); }

	void handleTKeyDown() { printf("Default handle T key down.\n"); }
	void handleTKeyHeld() { printf("Default handle T key held.\n"); }
	void handleTKeyUp() { printf("Default handle T key up.\n"); }

	void handleYKeyDown() { printf("Default handle Y key down.\n"); }
	void handleYKeyHeld() { printf("Default handle Y key held.\n"); }
	void handleYKeyUp() { printf("Default handle Y key up.\n"); }

	void handleUKeyDown() { printf("Default handle U key down.\n"); }
	void handleUKeyHeld() { printf("Default handle U key held.\n"); }
	void handleUKeyUp() { printf("Default handle U key up.\n"); }

	void handleIKeyDown() { printf("Default handle I key down.\n"); }
	void handleIKeyHeld() { printf("Default handle I key held.\n"); }
	void handleIKeyUp() { printf("Default handle I key up.\n"); }

	void handleOKeyDown() { printf("Default handle O key down.\n"); }
	void handleOKeyHeld() { printf("Default handle O key held.\n"); }
	void handleOKeyUp() { printf("Default handle O key up.\n"); }

	void handlePKeyDown() { printf("Default handle P key down.\n"); }
	void handlePKeyHeld() { printf("Default handle P key held.\n"); }
	void handlePKeyUp() { printf("Default handle P key up.\n"); }

	void handleAKeyDown() { printf("Default handle A key down.\n"); }
	void handleAKeyHeld() { printf("Default handle A key held.\n"); }
	void handleAKeyUp() { printf("Default handle A key up.\n"); }

	void handleSKeyDown() { printf("Default handle S key down.\n"); }
	void handleSKeyHeld() { printf("Default handle S key held.\n"); }
	void handleSKeyUp() { printf("Default handle S key up.\n"); }

	void handleDKeyDown() { printf("Default handle D key down.\n"); }
	void handleDKeyHeld() { printf("Default handle D key held.\n"); }
	void handleDKeyUp() { printf("Default handle D key up.\n"); }

	void handleFKeyDown() { printf("Default handle F key down.\n"); }
	void handleFKeyHeld() { printf("Default handle F key held.\n"); }
	void handleFKeyUp() { printf("Default handle F key up.\n"); }

	void handleGKeyDown() { printf("Default handle G key down.\n"); }
	void handleGKeyHeld() { printf("Default handle G key held.\n"); }
	void handleGKeyUp() { printf("Default handle G key up.\n"); }

	void handleHKeyDown() { printf("Default handle H key down.\n"); }
	void handleHKeyHeld() { printf("Default handle H key held.\n"); }
	void handleHKeyUp() { printf("Default handle H key up.\n"); }

	void handleJKeyDown() { printf("Default handle J key down.\n"); }
	void handleJKeyHeld() { printf("Default handle J key held.\n"); }
	void handleJKeyUp() { printf("Default handle J key up.\n"); }

	void handleKKeyDown() { printf("Default handle K key down.\n"); }
	void handleKKeyHeld() { printf("Default handle K key held.\n"); }
	void handleKKeyUp() { printf("Default handle K key up.\n"); }

	void handleLKeyDown() { printf("Default handle L key down.\n"); }
	void handleLKeyHeld() { printf("Default handle L key held.\n"); }
	void handleLKeyUp() { printf("Default handle L key up.\n"); }

	void handleZKeyDown() { printf("Default handle Z key down.\n"); }
	void handleZKeyHeld() { printf("Default handle Z key held.\n"); }
	void handleZKeyUp() { printf("Default handle Z key up.\n"); }

	void handleXKeyDown() { printf("Default handle X key down.\n"); }
	void handleXKeyHeld() { printf("Default handle X key held.\n"); }
	void handleXKeyUp() { printf("Default handle X key up.\n"); }

	void handleCKeyDown() { printf("Default handle C key down.\n"); }
	void handleCKeyHeld() { printf("Default handle C key held.\n"); }
	void handleCKeyUp() { printf("Default handle C key up.\n"); }

	void handleVKeyDown() { printf("Default handle V key down.\n"); }
	void handleVKeyHeld() { printf("Default handle V key held.\n"); }
	void handleVKeyUp() { printf("Default handle V key up.\n"); }

	void handleBKeyDown() { printf("Default handle B key down.\n"); }
	void handleBKeyHeld() { printf("Default handle B key held.\n"); }
	void handleBKeyUp() { printf("Default handle B key up.\n"); }

	void handleNKeyDown() { printf("Default handle N key down.\n"); }
	void handleNKeyHeld() { printf("Default handle N key held.\n"); }
	void handleNKeyUp() { printf("Default handle N key up.\n"); }

	void handleMKeyDown() { printf("Default handle M key down.\n"); }
	void handleMKeyHeld() { printf("Default handle M key held.\n"); }
	void handleMKeyUp() { printf("Default handle M key up.\n"); }

	void handleLeftArrowKeyDown() { printf("Default handle left arrow key down.\n"); }
	void handleLeftArrowKeyHeld() { printf("Default handle left arrow key held.\n"); }
	void handleLeftArrowKeyUp() { printf("Default handle left arrow key up.\n"); }

	void handleUpArrowKeyDown() { printf("Default handle up arrow key down.\n"); }
	void handleUpArrowKeyHeld() { printf("Default handle up arrow key held.\n"); }
	void handleUpArrowKeyUp() { printf("Default handle up arrow key up.\n"); }

	void handleDownArrowKeyDown() { printf("Default handle down arrow key down.\n"); }
	void handleDownArrowKeyHeld() { printf("Default handle down arrow key held.\n"); }
	void handleDownArrowKeyUp() { printf("Default handle down arrow key up.\n"); }

	void handleRightArrowKeyDown() { printf("Default handle right arrow key down.\n"); }
	void handleRightArrowKeyHeld() { printf("Default handle right arrow key held.\n"); }
	void handleRightArrowKeyUp() { printf("Default handle right arrow key up.\n"); }


	void handleSpaceKeyDown() { printf("Default handle space key down.\n"); }
	void handleSpaceKeyHeld() { printf("Default handle space key held.\n"); }
	void handleSpaceKeyUp() { printf("Default handle space key up.\n"); }

	void handleTabKeyDown() { printf("Default handle tab key down.\n"); }
	void handleTabKeyHeld() { printf("Default handle tab key held.\n"); }
	void handleTabKeyUp() { printf("Default handle tab key up.\n"); }

	void handleCapsLockKeyDown() { printf("Default handle caps lock down.\n"); }
	void handleCapsLockKeyHeld() { printf("Default handle caps lock held.\n"); }
	void handleCapsLockKeyUp() { printf("Default handle caps lock up.\n"); }

	void handleLShiftKeyDown() { printf("Default handle lShift key down.\n"); }
	void handleLShiftKeyHeld() { printf("Default handle lShift key held.\n"); }
	void handleLShiftKeyUp() { printf("Default handle lShift key up.\n"); }

	void handleEnterKeyDown() { printf("Default handle enter key down.\n"); }
	void handleEnterKeyHeld() { printf("Default handle enter key held.\n"); }
	void handleEnterKeyUp() { printf("Default handle enter key up.\n"); }


	void handleMouseLeftDown() { printf("Default handle left mouse down.\n"); }
	void handleMouseLeftHeld() { printf("Default handle left mouse held.\n"); }
	void handleMouseLeftUp() { printf("Default handle left mouse up.\n"); }

	void handleMouseMiddleDown() { printf("Default handle middle mouse down.\n"); }
	void handleMouseMiddleHeld() { printf("Default handle middle mouse held.\n"); }
	void handleMouseMiddleUp() { printf("Default handle midde mouse up.\n"); }

	void handleMouseRightDown() { printf("Default handle right mouse down.\n"); }
	void handleMouseRightHeld() { printf("Default handle right mouse held.\n"); }
	void handleMouseRightUp() { printf("Default handle right mouse up.\n"); }

	void handleMouseScroll(int xDir, int yDir) { printf("Default handle mouse scroll. Direction: X: %d, Y: %d\n", xDir, yDir); }

	void handleMouseMove(int xPos, int yPos) { printf("Default handle mouse move. Position: X: %d, Y: %d\n", xPos, yPos); }
}
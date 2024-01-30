#pragma once

#include <queue>

#include "../../include/Events/EventHandlers.h"
#include "../../include/Events/DefaultEventHandlers.h"

namespace Engine
{
	EventHandlers::EventHandlers()
		: quit(handleQuitEvent),
		windowResize(handleWindowResize),
		equalsDown(handleEqualsDown), equalsHeld(handleEqualsHeld), equalsUp(handleEqualsUp),
		escapeKeyDown(handleEscapeKeyDown), escapeKeyHeld(handleEscapeKeyHeld), escapeKeyUp(handleEscapeKeyUp),
		zeroKeyDown(handleZeroKeyDown), zeroKeyHeld(handleZeroKeyDown), zeroKeyUp(handleZeroKeyUp),
		oneKeyDown(handleOneKeyDown), oneKeyHeld(handleOneKeyHeld), oneKeyUp(handleOneKeyUp),
		twoKeyDown(handleTwoKeyDown), twoKeyHeld(handleTwoKeyHeld), twoKeyUp(handleTwoKeyUp),
		threeKeyDown(handleThreeKeyDown), threeKeyHeld(handleThreeKeyHeld), threeKeyUp(handleThreeKeyUp),
		fourKeyDown(handleFourKeyDown), fourKeyHeld(handleFourKeyHeld), fourKeyUp(handleFourKeyUp),
		fiveKeyDown(handleFiveKeyDown), fiveKeyHeld(handleFiveKeyHeld), fiveKeyUp(handleFiveKeyUp),
		sixKeyDown(handleSixKeyDown), sixKeyHeld(handleSixKeyHeld), sixKeyUp(handleSixKeyUp),
		sevenKeyDown(handleSevenKeyDown), sevenKeyHeld(handleSevenKeyHeld), sevenKeyUp(handleSevenKeyUp),
		eightKeyDown(handleEightKeyDown), eightKeyHeld(handleEightKeyHeld), eightKeyUp(handleEightKeyUp),
		nineKeyDown(handleNineKeyDown), nineKeyHeld(handleNineKeyHeld), nineKeyUp(handleNineKeyUp),
		qKeyDown(handleQKeyDown), qKeyHeld(handleQKeyHeld), qKeyUp(handleQKeyUp),
		wKeyDown(handleWKeyDown), wKeyHeld(handleWKeyHeld), wKeyUp(handleWKeyUp),
		eKeyDown(handleEKeyDown), eKeyHeld(handleEKeyHeld), eKeyUp(handleEKeyUp),
		rKeyDown(handleRKeyDown), rKeyHeld(handleRKeyHeld), rKeyUp(handleRKeyUp),
		tKeyDown(handleTKeyDown), tKeyHeld(handleTKeyHeld), tKeyUp(handleTKeyUp),
		yKeyDown(handleYKeyDown), yKeyHeld(handleYKeyHeld), yKeyUp(handleYKeyUp),
		uKeyDown(handleUKeyDown), uKeyHeld(handleUKeyHeld), uKeyUp(handleUKeyUp),
		iKeyDown(handleIKeyDown), iKeyHeld(handleIKeyHeld), iKeyUp(handleIKeyUp),
		oKeyDown(handleOKeyDown), oKeyHeld(handleOKeyHeld), oKeyUp(handleOKeyUp),
		pKeyDown(handlePKeyDown), pKeyHeld(handlePKeyHeld), pKeyUp(handlePKeyUp),
		aKeyDown(handleAKeyDown), aKeyHeld(handleAKeyHeld), aKeyUp(handleAKeyUp),
		sKeyDown(handleSKeyDown), sKeyHeld(handleSKeyHeld), sKeyUp(handleSKeyUp),
		dKeyDown(handleDKeyDown), dKeyHeld(handleDKeyHeld), dKeyUp(handleDKeyUp),
		fKeyDown(handleFKeyDown), fKeyHeld(handleFKeyHeld), fKeyUp(handleFKeyUp),
		gKeyDown(handleGKeyDown), gKeyHeld(handleGKeyHeld), gKeyUp(handleGKeyUp),
		hKeyDown(handleHKeyDown), hKeyHeld(handleHKeyHeld), hKeyUp(handleHKeyUp),
		jKeyDown(handleJKeyDown), jKeyHeld(handleJKeyHeld), jKeyUp(handleJKeyUp),
		kKeyDown(handleKKeyDown), kKeyHeld(handleKKeyHeld), kKeyUp(handleKKeyUp),
		lKeyDown(handleLKeyDown), lKeyHeld(handleLKeyHeld), lKeyUp(handleLKeyUp),
		zKeyDown(handleZKeyDown), zKeyHeld(handleZKeyHeld), zKeyUp(handleZKeyUp),
		xKeyDown(handleXKeyDown), xKeyHeld(handleXKeyHeld), xKeyUp(handleXKeyUp),
		cKeyDown(handleCKeyDown), cKeyHeld(handleCKeyHeld), cKeyUp(handleCKeyUp),
		vKeyDown(handleVKeyDown), vKeyHeld(handleVKeyHeld), vKeyUp(handleVKeyUp),
		bKeyDown(handleBKeyDown), bKeyHeld(handleBKeyHeld), bKeyUp(handleBKeyUp),
		nKeyDown(handleNKeyDown), nKeyHeld(handleNKeyHeld), nKeyUp(handleNKeyUp),
		mKeyDown(handleMKeyDown), mKeyHeld(handleMKeyHeld), mKeyUp(handleMKeyUp),
		leftArrowKeyDown(handleLeftArrowKeyDown), leftArrowKeyHeld(handleLeftArrowKeyHeld), leftArrowKeyUp(handleLeftArrowKeyUp),
		upArrowKeyDown(handleUpArrowKeyDown), upArrowKeyHeld(handleUpArrowKeyHeld), upArrowKeyUp(handleUpArrowKeyUp),
		downArrowKeyDown(handleDownArrowKeyDown), downArrowKeyHeld(handleDownArrowKeyHeld), downArrowKeyUp(handleDownArrowKeyUp),
		rightArrowKeyDown(handleRightArrowKeyDown), rightArrowKeyHeld(handleRightArrowKeyHeld), rightArrowKeyUp(handleRightArrowKeyUp),
		tabKeyDown(handleTabKeyDown), tabKeyHeld(handleTabKeyHeld), tabKeyUp(handleTabKeyUp),
		capslockKeyDown(handleCapsLockKeyDown), capslockKeyHeld(handleCapsLockKeyHeld), capslockKeyUp(handleCapsLockKeyUp),
		lShiftKeyDown(handleLShiftKeyDown), lShiftKeyHeld(handleLShiftKeyHeld), lShiftKeyUp(handleLShiftKeyUp),
		enterKeyDown(handleEnterKeyDown), enterKeyHeld(handleEnterKeyHeld), enterKeyUp(handleEnterKeyUp),
		spaceKeyDown(handleSpaceKeyDown), spaceKeyHeld(handleSpaceKeyHeld), spaceKeyUp(handleSpaceKeyUp),
		leftMouseButtonDown(handleMouseLeftDown), leftMouseButtonHeld(handleMouseLeftHeld), leftMouseButtonUp(handleMouseLeftUp),
		middleMouseButtonDown(handleMouseMiddleDown), middleMouseButtonHeld(handleMouseMiddleHeld), middleMouseButtonUp(handleMouseMiddleUp),
		rightMouseButtonDown(handleMouseRightDown), rightMouseButtonHeld(handleMouseRightHeld), rightMouseButtonUp(handleMouseRightUp),
		mouseMove(handleMouseMove),
		mouseScroll(handleMouseScroll)
	{}
}
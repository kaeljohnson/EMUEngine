#pragma once

#include <queue>

#include "../../include/Events/EventHandlers.h"
#include "../../include/Events/DefaultEventHandlers.h"

namespace Engine
{
	EventHandlers::EventHandlers()
		: quit(handleQuitEvent),
		windowResize(handleWindowResize),
		equalsDown(handleEqualsDown), equalsUp(handleEqualsUp),
		escapeKeyDown(handleEscapeKeyDown), escapeKeyUp(handleEscapeKeyUp),
		zeroKeyDown(handleZeroKeyDown), zeroKeyUp(handleZeroKeyUp),
		oneKeyDown(handleOneKeyDown), oneKeyUp(handleOneKeyUp),
		twoKeyDown(handleTwoKeyDown), twoKeyUp(handleTwoKeyUp),
		threeKeyDown(handleThreeKeyDown), threeKeyUp(handleThreeKeyUp),
		fourKeyDown(handleFourKeyDown), fourKeyUp(handleFourKeyUp),
		fiveKeyDown(handleFiveKeyDown), fiveKeyUp(handleFiveKeyUp),
		sixKeyDown(handleSixKeyDown), sixKeyUp(handleSixKeyUp),
		sevenKeyDown(handleSevenKeyDown), sevenKeyUp(handleSevenKeyUp),
		eightKeyDown(handleEightKeyDown), eightKeyUp(handleEightKeyUp),
		nineKeyDown(handleNineKeyDown), nineKeyUp(handleNineKeyUp),
		qKeyDown(handleQKeyDown), qKeyUp(handleQKeyUp),
		wKeyDown(handleWKeyDown), wKeyUp(handleWKeyUp),
		eKeyDown(handleEKeyDown), eKeyUp(handleEKeyUp),
		rKeyDown(handleRKeyDown), rKeyUp(handleRKeyUp),
		tKeyDown(handleTKeyDown), tKeyUp(handleTKeyUp),
		yKeyDown(handleYKeyDown), yKeyUp(handleYKeyUp),
		uKeyDown(handleUKeyDown), uKeyUp(handleUKeyUp),
		iKeyDown(handleIKeyDown), iKeyUp(handleIKeyUp),
		oKeyDown(handleOKeyDown), oKeyUp(handleOKeyUp),
		pKeyDown(handlePKeyDown), pKeyUp(handlePKeyUp),
		aKeyDown(handleAKeyDown), aKeyUp(handleAKeyUp),
		sKeyDown(handleSKeyDown), sKeyUp(handleSKeyUp),
		dKeyDown(handleDKeyDown), dKeyUp(handleDKeyUp),
		fKeyDown(handleFKeyDown), fKeyUp(handleFKeyUp),
		gKeyDown(handleGKeyDown), gKeyUp(handleGKeyUp),
		hKeyDown(handleHKeyDown), hKeyUp(handleHKeyUp),
		jKeyDown(handleJKeyDown), jKeyUp(handleJKeyUp),
		kKeyDown(handleKKeyDown), kKeyUp(handleKKeyUp),
		lKeyDown(handleLKeyDown), lKeyUp(handleLKeyUp),
		zKeyDown(handleZKeyDown), zKeyUp(handleZKeyUp),
		xKeyDown(handleXKeyDown), xKeyUp(handleXKeyUp),
		cKeyDown(handleCKeyDown), cKeyUp(handleCKeyUp),
		vKeyDown(handleVKeyDown), vKeyUp(handleVKeyUp),
		bKeyDown(handleBKeyDown), bKeyUp(handleBKeyUp),
		nKeyDown(handleNKeyDown), nKeyUp(handleNKeyUp),
		mKeyDown(handleMKeyDown), mKeyUp(handleMKeyUp),
		leftArrowKeyDown(handleLeftArrowKeyDown), leftArrowKeyUp(handleLeftArrowKeyUp),
		upArrowKeyDown(handleUpArrowKeyDown), upArrowKeyUp(handleUpArrowKeyUp),
		downArrowKeyDown(handleDownArrowKeyDown), downArrowKeyUp(handleDownArrowKeyUp),
		rightArrowKeyDown(handleRightArrowKeyDown), rightArrowKeyUp(handleRightArrowKeyUp),
		tabKeyDown(handleTabKeyDown), tabKeyUp(handleTabKeyUp),
		capslockKeyDown(handleCapsLockKeyDown), capslockKeyUp(handleCapsLockKeyUp),
		lShiftKeyDown(handleLShiftKeyDown), lShiftKeyUp(handleLShiftKeyUp),
		enterKeyDown(handleEnterKeyDown), enterKeyUp(handleEnterKeyUp),
		spaceKeyDown(handleSpaceKeyDown), spaceKeyUp(handleSpaceKeyUp),
		leftMouseButtonDown(handleMouseLeftDown), leftMouseButtonUp(handleMouseLeftUp),
		middleMouseButtonDown(handleMouseMiddleDown), middleMouseButtonUp(handleMouseMiddleUp),
		rightMouseButtonDown(handleMouseRightDown), rightMouseButtonUp(handleMouseRightUp),
		mouseMove(handleMouseMove),
		mouseScroll(handleMouseScroll)
	{}
}
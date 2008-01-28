//****************************************************************************
// "Those Funny Funguloids!"
// http://funguloids.sourceforge.net
// Copyright (c) 2006-2007, Mika Halttunen
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the
// use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software in a
//  product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and must not
//  be misrepresented as being the original software.
//
//  3. This notice may not be removed or altered from any source distribution.
//
//***************************************************************************/

#include "Game.h"
#include "Input.h"
#include <OgreStringConverter.h>


InputHandler::InputHandler(OgreAppFrameListener *listener, size_t hWnd) {
	mFrameListener = listener;

	OIS::ParamList pl;
	pl.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

	mHWnd = hWnd;
	mInputManager = OIS::InputManager::createInputSystem(pl);
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	mLMouseDown = mRMouseDown = mMMouseDown = false;
	for(int i=0; i<256; i++)
		mKeyDown[i] = false;
}


InputHandler::~InputHandler() {
/*	if(mMouse)
		delete mMouse;
	if(mKeyboard)
		delete mKeyboard;*/
	OIS::InputManager::destroyInputSystem(mInputManager);
	mMouse = 0;
	mKeyboard = 0;
}


void InputHandler::capture() {
	mMouse->capture();
	mKeyboard->capture();
}


void InputHandler::setWindowExtents(int width, int height) {
	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}


// MouseListener
bool InputHandler::mouseMoved(const OIS::MouseEvent &evt) {
	return true;
}

bool InputHandler::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID btn) {

	// Left mouse button down
	if(btn == OIS::MB_Left)
		mLMouseDown = true;

	// Right mouse button down
	else if(btn == OIS::MB_Right)
		mRMouseDown = true;

	// Middle mouse button down
	else if(btn == OIS::MB_Middle)
		mMMouseDown = true;
	return true;
}

bool InputHandler::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID btn) {

	// Left mouse button up
	if(btn == OIS::MB_Left)
		mLMouseDown = false;

	// Right mouse button up
	else if(btn == OIS::MB_Right)
		mRMouseDown = false;

	// Middle mouse button up
	else if(btn == OIS::MB_Middle)
		mMMouseDown = false;

	return true;
}


// KeyListener
bool InputHandler::keyPressed(const OIS::KeyEvent &evt) {
	mKeyDown[evt.key] = true;

	// Handle the key
	switch(evt.key) {
		case OIS::KC_F1:
			// Play a new track
			
			break;

		case OIS::KC_F:
			if(!mKeyboard->isModifierDown(OIS::Keyboard::Ctrl)) break;
			// Toggle the stats display
			
			break;

		case OIS::KC_SYSRQ:
			// Take a screenshot
			/*mFrameListener->saveScreenshot();
			gameApp->getRoot()->clearEventTimes();*/
			break;

		default: break;
	}
	return true;
}

bool InputHandler::keyReleased(const OIS::KeyEvent &evt) {
	mKeyDown[evt.key] = false;
	return true;
}

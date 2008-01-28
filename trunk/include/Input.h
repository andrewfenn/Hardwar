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

#ifndef INPUT_H
#define INPUT_H

//Use this define to signify OIS will be used as a DLL
//(so that dll import/export macros are in effect)
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

#include <OIS/OISEvents.h>
#include <OIS/OISInputManager.h>
#include <OIS/OISMouse.h>
#include <OIS/OISKeyboard.h>


class OgreAppFrameListener;

// Input handler class
class InputHandler : public OIS::KeyListener, OIS::MouseListener {
private:
	OIS::InputManager *mInputManager;
	OIS::Mouse *mMouse;
	OIS::Keyboard *mKeyboard;
	size_t mHWnd;
	OgreAppFrameListener *mFrameListener;

	bool mKeyDown[256];
	bool mLMouseDown, mRMouseDown, mMMouseDown;

public:
	InputHandler(OgreAppFrameListener *listener, size_t hWnd);
	~InputHandler();

	void setWindowExtents(int w, int h);
	void capture();

	bool isKeyDown(int key) const { return mKeyDown[key]; }
	bool isLMouseDown() const { return mLMouseDown; }
	bool isRMouseDown() const { return mRMouseDown; }
	bool isMMouseDown() const { return mMMouseDown; }

	// Mouse
	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID);

	// Keyboard
	bool keyPressed(const OIS::KeyEvent &evt);
	bool keyReleased(const OIS::KeyEvent &evt);
};


#endif

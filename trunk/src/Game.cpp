#include "Game.h"

bool Game::processUnbufferedMouseInput(const FrameEvent& evt)
{
	using namespace OIS;
	// Rotation factors, may not be used if the second mouse button is pressed
	// 2nd mouse button - slide, otherwise rotate
	const MouseState &ms = mMouse->getMouseState();
	if( ms.buttonDown( MB_Right ) ) {
		mTranslateVector.x += ms.X.rel * 0.13;
		mTranslateVector.y -= ms.Y.rel * 0.13;
	} else {
		mRotX = Degree(-ms.X.rel * 0.13);
		mRotY = Degree(-ms.Y.rel * 0.13);
	}
	return true;
}

bool Game::frameStarted(const FrameEvent &evt)
{
	mTimeSinceLastFrame = evt.timeSinceLastFrame;
   mMouse->capture();
	mKeyboard->capture();

	if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
     mQuitGame = 1;

   bool currMouse = mMouse->getMouseState().buttonDown(OIS::MB_Left);

   if (currMouse && ! mMouseDown)
   {
     Light *light = mSceneMgr->getLight("Light1");
     light->setVisible(! light->isVisible());
   }

   mMouseDown = currMouse;
   mToggle -= evt.timeSinceLastFrame;
	//mPlayer.processControls(mInput, mCamNode);

	if (mQuitGame == 1) return false;
	return true;
}


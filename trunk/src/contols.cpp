#include "ExampleApplication.h"

class TutorialFrameListener : public ExampleFrameListener
{
public:
    TutorialFrameListener( RenderWindow* win, Camera* cam, SceneManager *sceneMgr )
        : ExampleFrameListener(win, cam, false, false)
    {
        // key and mouse state tracking
        mMouseDown = false;
        mToggle = 0.0;

        // Populate the camera and scene manager containers
        mCamNode = cam->getParentSceneNode();
        mSceneMgr = sceneMgr;

        // set the rotation and move speed
        mRotate = 0.13;
        mMove = 250;
	//	mConsoleWait = 0;
		mQuitGame = 0;
    }

	bool processUnbufferedMouseInput(const FrameEvent& evt)
	{
		using namespace OIS;

		// Rotation factors, may not be used if the second mouse button is pressed
		// 2nd mouse button - slide, otherwise rotate
		const MouseState &ms = mMouse->getMouseState();
		if( ms.buttonDown( MB_Right ) )
		{
			mTranslateVector.x += ms.X.rel * 0.13;
			mTranslateVector.y -= ms.Y.rel * 0.13;
		}
		else
		{
			mRotX = Degree(-ms.X.rel * 0.13);
			mRotY = Degree(-ms.Y.rel * 0.13);
		}

		return true;
	}

    bool frameStarted(const FrameEvent &evt)
    {
        mMouse->capture();
		mKeyboard->capture();

		if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
            mQuitGame = 1;

//		, mKeyboard->getID();

		// check for console key press
	/*	if (mConsoleWait == 0) 
		{
			if (mKeyboard->isKeyDown(OIS::KC_0))
			{
				
				if (OgreConsole::getSingleton().isVisible())
				{
					OgreConsole::getSingleton().setVisible(false);
				} else {
					OgreConsole::getSingleton().setVisible(true);
				}
			}
			mConsoleWait = 30; // how long to count down before the button to be pushed again
		} else {
			mConsoleWait--;
		}
		*/


        bool currMouse = mMouse->getMouseState().buttonDown(OIS::MB_Left);

        if (currMouse && ! mMouseDown)
        {
            Light *light = mSceneMgr->getLight("Light1");
            light->setVisible(! light->isVisible());
        } // if

        mMouseDown = currMouse;
        mToggle -= evt.timeSinceLastFrame;

        if ((mToggle < 0.0f ) && mKeyboard->isKeyDown(OIS::KC_1))
        {
            mToggle = 0.5f;
            mCamera->getParentSceneNode()->detachObject(mCamera);
            mCamNode = mSceneMgr->getSceneNode("CamNode1");
            mCamNode->attachObject(mCamera);
        }

        else if ((mToggle < 0.0f) && mKeyboard->isKeyDown(OIS::KC_2))
        {
            mToggle = 0.5f;
            mCamera->getParentSceneNode()->detachObject(mCamera);
            mCamNode = mSceneMgr->getSceneNode("CamNode2");
            mCamNode->attachObject(mCamera);
        }

        Vector3 transVector = Vector3::ZERO;

        if (mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_W))
            transVector.z -= mMove;
        if (mKeyboard->isKeyDown(OIS::KC_DOWN) || mKeyboard->isKeyDown(OIS::KC_S))
            transVector.z += mMove;

        if (mKeyboard->isKeyDown(OIS::KC_LEFT) || mKeyboard->isKeyDown(OIS::KC_A))
            transVector.x -= mMove;
        if (mKeyboard->isKeyDown(OIS::KC_RIGHT) || mKeyboard->isKeyDown(OIS::KC_D))
            transVector.x += mMove;

        if (mKeyboard->isKeyDown(OIS::KC_PGUP) || mKeyboard->isKeyDown(OIS::KC_Q))
            transVector.y += mMove;
        if (mKeyboard->isKeyDown(OIS::KC_PGDOWN) || mKeyboard->isKeyDown(OIS::KC_E))
            transVector.y -= mMove;

        mCamNode->translate(transVector * evt.timeSinceLastFrame, Node::TS_LOCAL);

        // Do not add this to the program
       // mCamNode->translate(mCamNode->getOrientation() * transVector * evt.timeSinceLastFrame);

        if (mMouse->getMouseState().buttonDown(OIS::MB_Right))
        {
            mCamNode->yaw(Degree(-mRotate * mMouse->getMouseState().X.rel), Node::TS_WORLD);
            mCamNode->pitch(Degree(-mRotate * mMouse->getMouseState().Y.rel), Node::TS_LOCAL);
        }

		if (mQuitGame == 1) return false;
        return true;
    }
protected:
    bool mMouseDown;       // Whether or not the left mouse button was down last frame
    Real mToggle;          // The time left until next toggle
    Real mRotate;          // The rotate constant
    Real mMove;            // The movement constant
    SceneManager *mSceneMgr;   // The current SceneManager
    SceneNode *mCamNode;   // The SceneNode the camera is currently attached to
	//int mConsoleWait; // timer till you can open/close console
	int mQuitGame;
};

#ifndef __HW_GAME_H__
#define __HW_GAME_H__

#include "ExampleFrameListener.h"
#include "Input.h"
#include "Player.h"

class Game : public ExampleFrameListener
{
public:
    Game( RenderWindow* win, Camera* cam, SceneManager *sceneMgr )
        : ExampleFrameListener(win, cam, false, false)
    {
        // key and mouse state tracking
        mMouseDown = false;
        mToggle = 0.0;

        // Populate the camera and scene manager containers
        mCamNode = cam->getParentSceneNode();
        mSceneMgr = sceneMgr;

		  //	mConsoleWait = 0;
		  mQuitGame = 0;
		  
		  mPlayer = Player();
    }

	bool processUnbufferedMouseInput(const FrameEvent&);
   bool frameStarted(const FrameEvent&);
private:

protected:
 	Real mTimeSinceLastFrame;
   bool mMouseDown;       // Whether or not the left mouse button was down last frame
   Real mToggle;          // The time left until next toggle
   SceneManager *mSceneMgr;   // The current SceneManager
   SceneNode *mCamNode;   // The SceneNode the camera is currently attached to
   Camera *mCamera;
	int mQuitGame;
	Player mPlayer;
	InputHandler *mInput;
};

#endif

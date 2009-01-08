/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright (C) 2008  Andrew Fenn
    
    Hardwar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Hardwar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "PlayState.h"
#include "EditorState.h"
#include <OgreTextureUnitState.h>

using namespace Ogre;

PlayState* PlayState::mPlayState;

void PlayState::enter( void ) {

    mRoot         = Ogre::Root::getSingletonPtr();
	mRoot->createSceneManager(Ogre::ST_GENERIC, "PlaySceneMgr" );
	
	mOverlayMgr   = Ogre::OverlayManager::getSingletonPtr();
    mWindow       = mRoot->getAutoCreatedWindow();
	mSceneMgr     = mRoot->getSceneManager( "PlaySceneMgr" );
	mCamera       = mSceneMgr->createCamera( "PlayerCamera" );
	mViewport     = mWindow->addViewport( mCamera, 0 );
	mGUIMgr		  = GUIManager::getSingletonPtr();
    mConsole      = new GUIConsole;
    mOgreMax      = new OgreMax::OgreMaxScene;

    mNetwork      = new Client;

    if (!setupNetwork())
    {
        /* Networking connection has failed, shut down */
        std::cerr << "Error Connecting to server" << std::endl;
    }

	/* Initialise CEGUI for user interface stuff */
	mGUIMgr->initialise(mSceneMgr, mWindow);

    /* Initialise the console and add commands */
    mConsole->init(mRoot);
    mConsole->addCommand("addBuilding", PlayState::switchToEditor);

    mSceneMgr->setAmbientLight(Ogre::ColourValue::White);

    /* Setup our camera position in the world, we use the following coords
     * below because it is inside the model
     */
    mCamera->setPosition(Ogre::Vector3(43027.3, 1414.33, -30462.4));
    mCamera->setNearClipDistance(5);

    mWorldNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("world");
    mWorldNode->attachObject(mCamera);


   /* mWaterNode = mWorldNode->createChildSceneNode("water"); */

    const Ogre::String filename = Ogre::String("../media/hardwar/non-free/world.scene");
/*    OgreMax::OgreMaxSceneCallback* callback;*/

    /* load world */
    mOgreMax->Load(filename, mWindow, 0, mSceneMgr, mWorldNode);

    mMouseRotX = mMouseRotY = 0;
    mKeydownUp = mKeydownDown = mKeydownRight = mKeydownLeft = 0;
}

bool PlayState::setupNetwork()
{
    /* get arguements parsed at commandline from gamemanager */
    GameManager *gameManager = GameManager::getSingletonPtr();
    
    /* If single player game then start a server and connect to it */
    if (gameManager->mSinglePlayer)
    {
        std::cout << "Starting Singleplayer server" << std::endl;
        return true;
    }
    else
    {
        if (mNetwork->connect(gameManager->mPort, gameManager->mAddress))
        {
            /* request a position */
            mNetwork->message("packet", strlen("packet")+1, 0, ENET_PACKET_FLAG_RELIABLE);
            return true;
        }
    }
    return false;
}

void PlayState::exit( void )
{
    mSceneMgr->clearScene();
    mSceneMgr->destroyAllCameras();
    mRoot->getAutoCreatedWindow()->removeAllViewports();
}

void PlayState::switchToEditor(vector<String>& Params)
{
    PlayState* play = PlayState::getSingletonPtr();
    play->mConsole->setVisible(false);
    play->changeState( EditorState::getSingletonPtr());
}

void PlayState::pause( void )
{
}

void PlayState::resume( void )
{
}

void PlayState::update( unsigned long lTimeElapsed )
{
    fpstimer += lTimeElapsed;
    if (fpstimer > 0.5)
    {
        fpstimer = 0;

        static Ogre::String currFps = "Current FPS: ";
        static Ogre::String avgFps = "Average FPS: ";
        static Ogre::String worstFps = "X: ";
        static Ogre::String bestFps = "Y: ";
        static Ogre::String tris = "Z: ";

        /* update stats when necessary */
        Ogre::RenderWindow* rw = mRoot->getAutoCreatedWindow();
        Ogre::OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
        Ogre::OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
        Ogre::OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");
        Ogre::OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
        Ogre::OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
        Ogre::OverlayElement* guiBatch = OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
        Ogre::OverlayElement* guiImage = OverlayManager::getSingleton().getOverlayElement("Core/LogoPanel");
        guiImage->hide();

        guiAvg->setCaption(avgFps + StringConverter::toString(rw->getAverageFPS()));
        guiCurr->setCaption(currFps + StringConverter::toString(rw->getLastFPS()));
        guiBest->setCaption(bestFps + StringConverter::toString(mCamera->getPosition().y));
        guiWorst->setCaption(worstFps + StringConverter::toString(mCamera->getPosition().x));
        guiTris->setCaption(tris + StringConverter::toString(mCamera->getPosition().z));
        guiBatch->setCaption("Nothing to see here");
        Ogre::Overlay* o = OverlayManager::getSingleton().getByName("Core/DebugOverlay");

        if (!o)
        {
            OGRE_EXCEPT( Exception::ERR_ITEM_NOT_FOUND, "Could not find overlay Core/DebugOverlay", "showDebugOverlay" );
        }
        else
        {
            o->show();
        }
    }

    Ogre::Vector3 translateVector = Ogre::Vector3::ZERO;
    float scale = 5.0f;

    if (mKeydownUp == 1)
        translateVector.z -= scale;

    if (mKeydownDown == 1)
        translateVector.z += scale;

    if (mKeydownLeft == 1)
        translateVector.x -= scale;

    if (mKeydownRight == 1)
        translateVector.x += scale;

    mCamera->yaw(mMouseRotX);
    mCamera->pitch(mMouseRotY);    
    translateVector = mCamera->getOrientation() * translateVector;
    mCamera->setPosition(mCamera->getPosition()+translateVector);

    mMouseRotX = mMouseRotY = 0;
}

void PlayState::keyPressed( const OIS::KeyEvent &e )
{

    if (e.key == OIS::KC_GRAVE)
    {
        if (mConsole->isVisible())
        {
            mConsole->setVisible(false);
        }
        else
        {
            mConsole->setVisible(true);
        }
    }

    if (mConsole->isVisible())
    {
        /* Redirect input to the console */
        mConsole->onKeyPressed(e);
    }
    else
    {
        if (e.key == OIS::KC_W)
        {
            mKeydownUp = 1;
        }
        if (e.key == OIS::KC_S)
        {
            mKeydownDown = 1;
        }
        if (e.key == OIS::KC_A)
        {
            mKeydownLeft = 1;
        }
        if (e.key == OIS::KC_D)
        {
            mKeydownRight = 1;
        }
        if(e.key == OIS::KC_B)
        {
    		mSceneMgr->showBoundingBoxes(true);
    	}
    }
}

void PlayState::keyReleased( const OIS::KeyEvent &e )
{
    if( e.key == OIS::KC_ESCAPE )
    {
        this->requestShutdown();
    }
    if (e.key == OIS::KC_W)
    {
        mKeydownUp = 0;
    }
    if (e.key == OIS::KC_S)
    {
        mKeydownDown = 0;
    }
    if (e.key == OIS::KC_A)
    {
        mKeydownLeft = 0;
    }
    if (e.key == OIS::KC_D)
    {
        mKeydownRight = 0;
    }
}

void PlayState::mouseMoved( const OIS::MouseEvent &e )
{
    const OIS::MouseState &mouseState = e.state;

    mMouseRotX = Ogre::Degree(-mouseState.X.rel * 0.13);
    mMouseRotY = Ogre::Degree(-mouseState.Y.rel * 0.13);
        
    /*CEGUI::System::getSingleton().injectMousePosition(mouseState.X.abs, mouseState.Y.abs);*/
}

void PlayState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
 	/*CEGUI::System::getSingleton().injectMouseButtonDown((CEGUI::MouseButton)id);*/
}

void PlayState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
}

PlayState* PlayState::getSingletonPtr( void )
{
    if( !mPlayState )
    {
        mPlayState = new PlayState();
    }
    return mPlayState;
}


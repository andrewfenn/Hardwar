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

#include "LoadState.h"

LoadState* LoadState::mLoadState;

void LoadState::enter( void )
{
    mRoot            = Ogre::Root::getSingletonPtr();
	mRoot->createSceneManager(Ogre::ST_GENERIC,"LoadSceneMgr");

	mOverlayMgr   = Ogre::OverlayManager::getSingletonPtr();
    mWindow       = mRoot->getAutoCreatedWindow();
	mSceneMgr     = mRoot->getSceneManager("LoadSceneMgr");
	mCamera       = mSceneMgr->createCamera("PlayerCamera");
	mViewport     = mWindow->addViewport(mCamera, 0 );
    mGameMgr      = GameManager::getSingletonPtr();

    Ogre::Overlay* o = Ogre::OverlayManager::getSingleton().getByName("Core/LoadOverlay");

    if (!o)
    {
        OGRE_EXCEPT( Ogre::Exception::ERR_ITEM_NOT_FOUND, "Could not find overlay Core/LoadOverlay", "LoadState.cpp::enter()");
    }
    else
    {
        o->show();
    }

    mReverse = false; /* for the load bar animation */
    Ogre::OverlayElement* loadstats = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/LoadPanel/Comment");

    if (mGameMgr->mSinglePlayer)
    {
        loadstats->setCaption(Ogre::String("Starting game"));
    }
    else
    {
        loadstats->setCaption(Ogre::String("Connecting to server"));
    }

    mConAttempts = 0; /* connection attempts */
    mCounter = 101; /* timer for a delay between connection attempts */
}

void LoadState::exit( void )
{
    Ogre::Overlay* o = Ogre::OverlayManager::getSingleton().getByName("Core/LoadOverlay");
    o->hide();
    mSceneMgr->destroyCamera(mCamera);
    mWindow->removeAllViewports();
    mRoot->destroySceneManager(mSceneMgr);
}

void LoadState::pause(void)
{}

void LoadState::resume(void)
{}

void LoadState::update( unsigned long lTimeElapsed )
{
    updateLoadbar(lTimeElapsed);

    
    
    if (mCounter > 1000)
    {
        Ogre::OverlayElement* loadstats = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/LoadPanel/Comment");
        if (mConAttempts < 10)
        {
            mCounter = 0;
            
            bool result = mGameMgr->setupNetwork();
            if (result)
            {
                if (mGameMgr->mSinglePlayer)
                {
                    loadstats->setCaption(Ogre::String("Connected to server"));
                }
                else
                {
                    loadstats->setCaption(Ogre::String("Started game"));
                }
                this->changeState(PlayState::getSingletonPtr());
            }
            else
            {
                mConAttempts++;
                if (mGameMgr->mSinglePlayer)
                {
                    loadstats->setCaption(Ogre::String("Problem starting game. (Attempts: ")+Ogre::StringConverter::toString(mConAttempts)+Ogre::String(")"));
                }
                else
                {
                    loadstats->setCaption(Ogre::String("Connection failed, Retrying. (Attempts: ")+Ogre::StringConverter::toString(mConAttempts)+Ogre::String(")"));
                }
            }
        }
        else
        {
            /* the connection has failed */
            killLoadbar();
        }
    }
    mCounter += lTimeElapsed;
}

void LoadState::updateLoadbar(unsigned long lTimeElapsed)
{
    Ogre::OverlayElement* progress = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/LoadPanel/Bar/Progress");
    Ogre::Real left = progress->getLeft();
    if (mReverse)
    {
        left -=(0.1*lTimeElapsed);
        if (left < 0)
        {
            mReverse = false;
            left = 0;
        }
    }
    else
    {
        left +=(0.1*lTimeElapsed);
        if (left > 350)
        {
            mReverse = true;
            left = 350;
        }
    }
    progress->setLeft(left);
}

void LoadState::killLoadbar()
{
    Ogre::OverlayElement* loadstats = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/LoadPanel/Comment");
    loadstats->setCaption(Ogre::String("Press ESC to quit"));
    Ogre::OverlayElement* progress = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/LoadPanel/Bar/Progress");
    progress->hide();
    Ogre::OverlayElement* desctext = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/LoadPanel/Description");
    if (mGameMgr->mSinglePlayer)
    {
        desctext->setCaption(Ogre::String("Failed to start game"));
    }
    else
    {
        desctext->setCaption(Ogre::String("Failed to connect to server"));
    }
}

void LoadState::keyPressed( const OIS::KeyEvent &e )
{
}

void LoadState::keyReleased( const OIS::KeyEvent &e )
{
    if( e.key == OIS::KC_ESCAPE )
    {
        this->requestShutdown();
    }

	if (e.key == OIS::KC_SYSRQ) { // screenshot
		char filename[30] ;

       
	   std::sprintf(filename, "./screenshots/screenshot.png");
	   mRoot->getAutoCreatedWindow()->writeContentsToFile(filename);
	}    
}

void LoadState::mouseMoved( const OIS::MouseEvent &e )
{

}

void LoadState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
}

void LoadState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
}

LoadState* LoadState::getSingletonPtr( void )
{
    if( !mLoadState ) {
        mLoadState = new LoadState();
    }

    return mLoadState;
}
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

    mWindow       = mRoot->getAutoCreatedWindow();
	mSceneMgr     = mRoot->getSceneManager("LoadSceneMgr");
	mCamera       = mSceneMgr->createCamera("LoadCamera");
	mViewport     = mWindow->addViewport(mCamera, 0 );
    mGameMgr      = GameManager::getSingletonPtr();

    mGUI = new MyGUI::Gui();
    mGUI->initialise(mWindow);
    mGUI->hidePointer();
    mReverse = false; /* for the load bar animation */

    /* Get MyGUI loading layout */
    MyGUI::LayoutManager::getInstance().load("loading.layout");
    mStatusText = MyGUI::Gui::getInstance().findWidget<MyGUI::StaticText>("status");

    if (mGameMgr->mSinglePlayer)
    {
        mStatusText->setCaption(Ogre::String(gettext("Starting game")));
    }
    else
    {
        mStatusText->setCaption(Ogre::String(gettext("Connecting to server")));
    }

    mConAttempts = 0; /* connection attempts so far */
    mCounter = 101; /* timer for a delay between connection attempts */

    mLoadStatus = STATUS_CONNECTING;
    mRetryLimit = 10; /* TODO: Make this config option */
    mTimeout = 30; /* TODO: Make this config option. */
    mGUIcount = 0;
    mReverse = false;
}

/* Destory everything we created when entering */
void LoadState::exit( void )
{
    /* Delete pointer */
    MyGUI::Gui::getInstance().destroyWidget(mStatusText);
    /* Delete MyGUI */
    mGUI->shutdown();
    delete mGUI;
    mGUI = 0;
    /* Delete the camera and scene */
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
    mGUICounter += lTimeElapsed;
    mCounter += lTimeElapsed;

    updateLoadbar();

    switch(mLoadStatus)
    {
        case STATUS_CONNECTING:
                connect();
        break;
        case STATUS_LISTENING:
                waitForReply();
        break;
        case STATUS_CONNECTED:
                this->changeState(PlayState::getSingletonPtr());
        break;
        default:
        break;
    }
}

void LoadState::waitForReply(void)
{
    ENetEvent event;
    if (mGameMgr->mNetwork->pollMessage(&event))
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_RECEIVE:
            {
                printf("A packet of length %u containing 0x%08x was received on channel %u.\n",
                    event.packet->dataLength,
                    (unsigned int)event.packet->data,
                    event.channelID);
                Player player;
                memcpy(&player, event.packet->data, sizeof(Player));
                printf("Packet contains name: \"%s\" connection state: 0x%08x was inside.\n",
                    player.name, player.conState);
                mLoadStatus = player.conState;
                // Clean up the packet now that we're done using it.
                enet_packet_destroy(event.packet);
            }
            break;
            default:
            break;
        }
    }
    else
    {
        unsigned int timeout = mTimeout - (mCounter*0.001);
        if (timeout < mTimeout-1)
        {
            mStatusText->setCaption(Ogre::String(gettext("Waiting for response. Timeout: "))+Ogre::StringConverter::toString(timeout));
            if (timeout == 0)
            {
                mLoadStatus = STATUS_CONNECTING;
                mConAttempts++;
            }
        }
    }
}

void LoadState::connect(void)
{
    if ((mCounter*0.001) > 1)
    {
        if (mConAttempts < mRetryLimit)
        {
            mCounter = 0;
            
            bool result = mGameMgr->setupNetwork();
            if (result)
            {
                if (mGameMgr->mSinglePlayer)
                {
                    mStatusText->setCaption(Ogre::String(gettext("Loading world")));
                }
                else
                {
                    mStatusText->setCaption(Ogre::String(gettext("Waiting for response")));
                }
                mLoadStatus = STATUS_LISTENING;
            }
            else
            {
                mConAttempts++;
                mStatusText->setCaption(Ogre::UTFString(gettext("Retrying"))+Ogre::UTFString(" (")+
                                  Ogre::StringConverter::toString(mConAttempts)+Ogre::UTFString(")"));

            }
        }
        else
        {
            /* the connection has failed */
            mLoadStatus = STATUS_DISCONNECTED;
            killLoadbar();
        }
    }
}

/* Update the GUI animation */
void LoadState::updateLoadbar(void)
{
    MyGUI::types::TCoord<Ogre::Real> coord;
    if (mGUICounter*0.001 > 0.5)
    {
        mGUICounter = 0;
        MyGUI::StaticImagePtr statusImage;

        if (mGUIcount > 0)
        {
            /* set the size of the last big dot to small */
            statusImage= MyGUI::Gui::getInstance().findWidget<MyGUI::StaticImage> (
                                                             Ogre::UTFString("dot")+
                                          Ogre::StringConverter::toString(mGUIcount)
                                                                                  );
            coord = statusImage->getCoord();
            statusImage->setCoord(coord.left+3, coord.top +3, coord.width -5, coord.height -5);
        }

        if (mReverse)
        {
            mGUIcount--;
        }
        else
        {
            mGUIcount++;
        }

        if (mGUIcount < 2)
        {
            mGUIcount = 1;
            mReverse = false;
        }
        if (mGUIcount > 2)
        {
            mGUIcount = 3;
            mReverse = true;
        }

        statusImage= MyGUI::Gui::getInstance().findWidget<MyGUI::StaticImage> (
                                                             Ogre::UTFString("dot")+
                                          Ogre::StringConverter::toString(mGUIcount)
                                                                                  );
        coord = statusImage->getCoord();
        statusImage->setCoord(coord.left-3, coord.top -3, coord.width +5, coord.height +5);
    }
}

/* Stop the loading animation and stop failure message */
void LoadState::killLoadbar()
{
    /* TODO: make this a popup window? */

    /*mStatusText->setCaption(Ogre::String(gettext("Press ESC to quit")));*/

    if (mGameMgr->mSinglePlayer)
    {
        mStatusText->setCaption(Ogre::String(gettext("Failed to start game")));
    }
    else
    {
        mStatusText->setCaption(Ogre::String(gettext("Failed to connect to server")));
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

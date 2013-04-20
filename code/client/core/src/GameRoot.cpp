/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2010-2011  Andrew Fenn
    
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


#include "GameRoot.h"

#include <fstream>
#include "OgreException.h"

namespace Client
{

GameRoot::GameRoot()
{
   mRoot = nullptr;
}

GameRoot::~GameRoot()
{
    Ogre::WindowEventUtilities::removeWindowEventListener(mRenderWindow, this);

    if(mRoot)
    {
        mRoot->getAutoCreatedWindow()->removeAllViewports();
        mRoot->shutdown();
        OGRE_DELETE mRoot;
        mRoot = 0;
    }
}

bool GameRoot::loadPlugin(const Ogre::String dir)
{
    if (mRoot == nullptr) {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "mRoot not initialised", "GameRoot::loadPlugin");
    }

    try
    {
        mRoot->loadPlugin(dir);
    }
    catch(Ogre::Exception& e)
    {
        Ogre::LogManager::getSingleton().logMessage(Ogre::String("Unable to load plugin: ") + e.getFullDescription());
        return false;
    }
    return true;
}

void GameRoot::init()
{
    Ogre::LogManager* log = OGRE_NEW Ogre::LogManager();
    auto logDir = this->getHomeDirectory() + Ogre::UTFString("/client.log");
    log->createLog(logDir, true, false);
    log->setLogDetail(Ogre::LL_BOREME);

    mRoot = OGRE_NEW Ogre::Root("", "game.cfg", "");

    if (!this->loadPlugins())
        return;
}

void GameRoot::run()
{
    if (!this->configureGame())
        return;

    if (!mRoot->isInitialised())
        return;

    mGameMgr = OGRE_NEW GameManager(mRoot);
    mGameMgr->run();
}

bool GameRoot::configureGame()
{
    /* Load config settings from ogre.cfg */
    if( !mRoot->restoreConfig() && !mRoot->showConfigDialog() )
    {
        /* If game can't be configured, throw exception and quit application */
        std::cerr << "Game can not be configured" << std::endl;
        return false;
    }

    /* Initialise and create a default rendering window */
    mRenderWindow = mRoot->initialise( true, "Hardwar" );
    Ogre::WindowEventUtilities::addWindowEventListener(mRenderWindow, this);

    return true;
}

void GameRoot::windowResized(Ogre::RenderWindow *rw)
{
    mGameMgr->windowChangedSize(rw);
}

bool GameRoot::windowClosing(Ogre::RenderWindow *rw)
{
    mGameMgr->shutdown();
    return true;
}

void GameRoot::windowFocusChange(Ogre::RenderWindow *rw)
{
    mGameMgr->windowChangedFocus(rw);
}
} /* namespace Client */


/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2008-2012  Andrew Fenn
    
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


#include "GameManager.h"

/* game tasks */
#include "InputTask.h"
//#include "NetworkTask.h"
//#include "ZoneTask.h"
//#include "GuiTask.h"

//#include "MenuState.h"
//#include "ConsoleState.h"

namespace Client
{

GameManager::GameManager(Ogre::Root* root)
{
    mShutdown = false;
    mRunning = false;
    mRoot = root;

    /* preload the resources that we're going to use before showing anything */
    this->preloadResources();

    mRenderWindow = mRoot->getAutoCreatedWindow();
    mSceneMgr     = mRoot->createSceneManager(Ogre::ST_GENERIC, "GameSceneMgr");
    mCamera       = mSceneMgr->createCamera("GameCamera");
    mViewport     = mRenderWindow->addViewport(mCamera, 1);
    mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));

    /* create root state */
    mRootState = OGRE_NEW RootGameState(&mTasks, mRoot, mViewport);

    /* attach game modules to root state */
    //mRootState->add(OGRE_NEW ConsoleState);
    //mRootState->add(OGRE_NEW MenuState);

    mMaxFPS = 60;
    mDelayTime = ceil( (float) 1000 / mMaxFPS );
}

GameManager::~GameManager()
{
    if (!mRunning)
        return;

    /* It's important that the below code remains here rather then in the
    shutdown function so that we know that the object isn't accessing the
    resources while we're trying to destroy them. */

    /* Clean up all the game states */
    if (mRootState)
        mRootState->shutdown();

    this->removeTasks();

    if (mSceneMgr)
    {
        mSceneMgr->clearScene();
        mSceneMgr->destroyAllCameras();
        mRenderWindow->removeAllViewports();
    }
}

void GameManager::createTasks()
{
   /* setup system tasks */
   mTasks.add("Input", OGRE_NEW InputTask);
   //mTasks.add("Network", OGRE_NEW NetworkTask);
   //mTasks.add("Zone", OGRE_NEW ZoneTask);
   //mTasks.add("Gui", OGRE_NEW GuiTask(mRenderWindow, mSceneMgr));
}

void GameManager::removeTasks()
{
    if (!mRunning)
        return;

    /* Clean up all the game tasks such as networking, etc. */
    mTasks.removeAll();
}

void GameManager::run()
{
    mRunning = true;

    TaskList* list = mTasks.list();
    TaskList::iterator taskItr;

    /* Following are used to calculate the time since last frame */
    unsigned long timeLastFrame      = 0,
                    timeCurrentFrame   = 0,
                    timeSinceLastFrame = 0,
                    delay              = 0;

    while (!mShutdown)
    {
        /* Calculate time since last frame and remember current time for next frame */
        timeCurrentFrame = mRoot->getTimer()->getMilliseconds();
        timeSinceLastFrame = timeCurrentFrame - timeLastFrame;
        timeLastFrame = timeCurrentFrame;

        delay += timeSinceLastFrame;

        if (delay > mDelayTime)
        {
            /* update game system tasks */
            for (taskItr=list->begin(); taskItr != list->end(); taskItr++)
            {
                taskItr->second->update();
            }

            /* update the current game states */
            mRootState->update(timeSinceLastFrame);

            /* render this frame */
            mRoot->renderOneFrame();
            delay = 0;
        }

        /* Deal with platform specific issues */
        Ogre::WindowEventUtilities::messagePump();

        mShutdown = mRootState->shouldExit();
    }
    mRunning = false;
}

void GameManager::windowChangedSize(Ogre::RenderWindow* rw)
{
   TaskList* list = mTasks.list();
   for (TaskList::iterator i = list->begin(); i != list->end(); i++)
   {
      i->second->changeSize(rw);
   }
}

void GameManager::windowChangedFocus(Ogre::RenderWindow* rw)
{
   TaskList* list = mTasks.list();
   for (TaskList::iterator i = list->begin(); i != list->end(); i++)
   {
      i->second->changeFocus(rw);
   }
}

void GameManager::preloadResources()
{
    /* Load resource paths from config file */
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    /* go through all settings in the file */
    Ogre::ConfigFile::SectionIterator itSection = cf.getSectionIterator();

    Ogre::String sSection, sType, sArch;
    while(itSection.hasMoreElements())
    {
        sSection = itSection.peekNextKey();

        Ogre::ConfigFile::SettingsMultiMap *mapSettings = itSection.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator itSetting = mapSettings->begin();
        while(itSetting != mapSettings->end())
        {
            sType = itSetting->first;
            sArch = itSetting->second;
            mRoot->addResourceLocation(sArch, sType, sSection);
            ++itSetting;
        }
    }

    /* Initialise resources */
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void GameManager::shutdown(void)
{
   mShutdown = true;
}

} /* namespace Client */


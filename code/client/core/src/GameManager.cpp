/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2008-2010  Andrew Fenn
    
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

namespace Client
{

GameManager::GameManager(Ogre::Root* root)
{
   mShutdown = false;
   mStarted = false;
   mRoot = root;
}

GameManager::~GameManager()
{
   if (!mStarted)
      return;

   /* Clean up all the game states */
   if (mRootState)
      mRootState->shutdown();

   /* Clean up all the game tasks
    * such as networking, etc.
    */
   mTasks.removeAll();

   if (mSceneMgr)
   {
      mSceneMgr->clearScene();
      mSceneMgr->destroyAllCameras();
      mRenderWindow->removeAllViewports();
   }
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

void GameManager::start()
{
   mRenderWindow = mRoot->getAutoCreatedWindow();
   mSceneMgr     = mRoot->createSceneManager(Ogre::ST_GENERIC, "GameSceneMgr");
   mCamera       = mSceneMgr->createCamera("GameCamera");
   mViewport     = mRenderWindow->addViewport(mCamera, 1);

   /* setup system tasks */
   mTasks.add("Input", OGRE_NEW InputTask);
   mTasks.add("Network", OGRE_NEW NetworkTask);
   mTasks.add("Zone", OGRE_NEW ZoneTask);
   mTasks.add("Gui", OGRE_NEW GuiTask(mRenderWindow, mSceneMgr));

   /* create root state */
   mRootState = OGRE_NEW RootGameState(&mTasks, mRoot, mViewport);

   /* attach game modules to root state */
   mRootState->add(OGRE_NEW LoadState);
   
   mStarted = true;

   /* 
    * lTimeLastFrame remembers the last time that it was checked
    * We use it to calculate the time since last frame
    */
   unsigned long lTimeLastFrame = 0,
                 lTimeCurrentFrame = 0,
                 lTimeSinceLastFrame = 0;
   unsigned long lDelay = 0;

   mMaxFPS = 60;
   mDelayTime = ceil((float)1000/mMaxFPS);

   TaskList* list = mTasks.list();

   while (!mShutdown)
   {
      /* 
       * Calculate time since last frame and remember current time 
       * for next frame 
       */
      lTimeCurrentFrame = mRoot->getTimer()->getMilliseconds();
      lTimeSinceLastFrame = lTimeCurrentFrame - lTimeLastFrame;
      lTimeLastFrame = lTimeCurrentFrame;

      lDelay += lTimeSinceLastFrame;
      if (lDelay > mDelayTime)
      {
         /* update game system tasks */
         for (TaskList::iterator i=list->begin(); i != list->end(); i++)
         {
            i->second->update();
         }

         /* update the current game states */
         mRootState->update(lTimeSinceLastFrame);

         /* render the next frame */
         mRoot->renderOneFrame();
         lDelay = 0;
      }
      /* Deal with platform specific issues */
      Ogre::WindowEventUtilities::messagePump();
   }
}

void GameManager::shutdown(void)
{
   mShutdown = true;
}

} /* namespace Client */


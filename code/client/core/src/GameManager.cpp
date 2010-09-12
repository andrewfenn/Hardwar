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
   mRoot = root;
}

GameManager::~GameManager( void )
{
   /* Clean up all the game states */
   mRootState.shutdown();

   /* Clean up all the game tasks
    * such as networking, etc.
    */
   for (GameTaskList::iterator i=mTasks.begin(); i != mTasks.end(); i++)
   {
      i->second->shutdown();
      mTasks.erase(i);
   }

   /* Delete MyGUI */
   mGUI->destroyAllChildWidget();
   mGUI->shutdown();
   OGRE_DELETE mGUI;
   mGUI = 0;   

   mPlatform->shutdown();
   OGRE_DELETE mPlatform;
   mPlatform = 0;

   mSceneMgr->clearScene();
   mSceneMgr->destroyAllCameras();
   mRenderWindow->removeAllViewports();
}

void GameManager::windowChangedSize(Ogre::RenderWindow* rw)
{
   mRenderWindow = rw;
   GameTaskList::iterator i = mTasks.begin();
   if (i != mTasks.end())
   {
      i->second->changeSize(rw);
   }
}

void GameManager::windowChangedFocus(Ogre::RenderWindow* rw)
{
   mRenderWindow = rw;
   GameTaskList::iterator i = mTasks.begin();
   if (i != mTasks.end())
   {
      i->second->changeFocus(rw);
   }
}

void GameManager::start()
{
   mRoot->createSceneManager(Ogre::ST_GENERIC, "GameSceneMgr");
   mSceneMgr     = mRoot->getSceneManager("GameSceneMgr");
   mCamera       = mSceneMgr->createCamera("GameCamera");
   mViewport     = mRenderWindow->addViewport(mCamera, 1);

   mGUI = OGRE_NEW MyGUI::Gui();
   mPlatform = OGRE_NEW MyGUI::OgrePlatform();
   mPlatform->initialise(mRenderWindow, mSceneMgr, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "");
   mGUI->initialise("core.xml", "./logs/mygui.log");

   /* setup system tasks */
   this->createTask("Input", OGRE_NEW InputTask);
   this->createTask("Network", OGRE_NEW NetworkTask);
   this->createTask("Zone", OGRE_NEW ZoneTask);

   /* create root state */
   mRootState.setTaskList(&mTasks);

   /* attach game modules to root state */
   mRootState.createState(OGRE_NEW LoadState);


   /* 
    * lTimeLastFrame remembers the last time that it was checked
    * We use it to calculate the time since last frame
    */
   unsigned long lTimeLastFrame, lTimeCurrentFrame, lTimeSinceLastFrame = 0;
   unsigned long lDelay = 0;

   mMaxFPS = 60;
   mDelayTime = ceil((float)1000/mMaxFPS);

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
         for (GameTaskList::iterator i=mTasks.begin(); i != mTasks.end(); i++)
         {
            i->second->update();
         }

         /* update the current game states */
         mRootState.update(lTimeSinceLastFrame);

         /* render the next frame */
         mRoot->renderOneFrame();
         lDelay = 0;
      }
      /* Deal with platform specific issues */
      Ogre::WindowEventUtilities::messagePump();
   }
}

GameTask* GameManager::createTask(const Ogre::String& name, GameTask* task)
{
   if (mTasks.find(name) != mTasks.end())
   {
      OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
         "A task with the name " + name + " already exists",
         "GameManager::createTask" );
   }
   task->setTaskList(&mTasks);
   mTasks.insert(GameTaskList::value_type(name, task));
   return task;
}

GameTask* GameManager::getTask(const Ogre::String& name)
{
   GameTaskList::const_iterator i = mTasks.find(name);
   if (i != mTasks.end())
   {
      return (GameTask*) &i->second;
   }
   OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
      "Cannot find Task with name " + name,
      "GameTask::getTask");
}

bool GameManager::hasTask(const Ogre::String& name)
{
   return (mTasks.find(name) != mTasks.end());
}

void GameManager::destroyTask(const Ogre::String& name)
{
   GameTaskList::iterator i = mTasks.find(name);
   if (i != mTasks.end())
   {
      OGRE_DELETE &i->second;
      mTasks.erase(i);
   }
}

void GameManager::shutdown(void)
{
   mShutdown = true;
}
} /* namespace Client */


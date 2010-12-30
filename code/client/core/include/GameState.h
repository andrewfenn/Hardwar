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

#pragma once

#include <Ogre.h>
#include "GameTask.h"

namespace Client
{
class GameState;
typedef std::map<Ogre::String, GameState*> GameStateList;

class GameState
{
public:
   GameState(const Ogre::String&);

   virtual void enter() {}
   virtual void exit() {}
   virtual void update(unsigned long lTimeElapsed);

   /** Internal only method for setting up the data in the state propperly */
   void setParent(GameState*, GameTaskList*, Ogre::Root*, Ogre::Viewport*);

   const Ogre::String getName();
   void* getParent();
   void shutdown();

   void* add(GameState*);
   void* get(const Ogre::String&);
   void replace(GameState*);
   bool has(const Ogre::String&);
   bool has(GameState&);
   void remove(const Ogre::String&);
   void removeAllChildren();
   void updateAllChildren(unsigned long lTimeElapsed);
   void pause();
   void resume();
protected:
   bool mPaused;
   GameState* mParent;
   GameStateList mChildren;
   Ogre::String mName;
   Ogre::Root           *mRoot;
   Ogre::SceneManager   *mSceneMgr;
   Ogre::Camera         *mCamera;
   Ogre::Viewport       *mViewport;
   GameTaskList         *mTasklist;
};

class RootGameState : public GameState
{
public:
   RootGameState(GameTaskList* gametasks, Ogre::Root* root, Ogre::Viewport* viewport) : GameState("Root")
   {
      mTasklist = gametasks;
      mRoot     = root;
      mSceneMgr = root->getSceneManager("GameSceneMgr");
      mCamera   = mSceneMgr->getCamera("GameCamera");
      mViewport = viewport;
      mShutdown = false;
   }
   
   void update(unsigned long lTimeElapsed)
   {
      this->updateAllChildren(lTimeElapsed);
   }
   
   const bool shouldExit()
   {
      return mShutdown;
   }
   
   void shutdown()
   {
      std::cout << "DEBUG: AT ROOT SHUTDOWN" << std::endl;
      mShutdown = true;
      this->removeAllChildren();
   }
private:
   /* if true shuts down the game */
   bool mShutdown;
};
}


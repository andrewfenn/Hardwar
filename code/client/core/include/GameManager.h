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

#include "GameState.h"
#include "GameTask.h"

namespace Client
{
/** The Game Manager
        @remarks
            The Client::GameManager class is our root class which deals with the
            initialisation and running of our game.
    */
class GameManager
{
   public:
      Ogre::SceneManager   *mSceneMgr;

      GameManager(Ogre::Root*);
      ~GameManager();

      void start();
      void windowChangedSize(Ogre::RenderWindow*);
      void windowChangedFocus(Ogre::RenderWindow*);
      void shutdown();

   private:
      Ogre::Root           *mRoot;
      Ogre::RenderWindow   *mRenderWindow;
      Ogre::Camera         *mCamera;
      Ogre::Viewport       *mViewport;
      RootGameState        *mRootState;

      bool                 mShutdown;
      bool                 mStarted;
      GameTaskList         mTasks;

      unsigned short       mDelayTime;
      unsigned short       mMaxFPS;
};
}


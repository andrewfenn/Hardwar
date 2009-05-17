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

#ifndef GameManager_H
#define GameManager_H

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>
#include <stdio.h>
#include <string.h>
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#include <dirent.h>
#endif
#include <MyGUI.h>

#include "InputManager.h"
#include "Network.h"
#include "Console.h"
#include "GameSettings.h"

namespace Client
{

class GameState;

class GameManager : public OIS::KeyListener, OIS::MouseListener
{
   public:
      bool                mSinglePlayer;
      Network             *mNetwork;

      ~GameManager(void);
      void startGame(GameState *gameState);
      void changeState(GameState *gameState);
      void pushState(GameState *gameState);
      void popState(void);
      void requestShutdown(void);

      static GameManager* getSingletonPtr(void);
      MyGUI::Gui           *mGUI;
      Console              *mConsole;
      GameSettings         *mSettings;

      Ogre::Camera            *mCamera;
      Ogre::SceneManager      *mSceneMgr;
      Ogre::Viewport          *mViewport;

   private:
      Ogre::Root           *mRoot;
      Ogre::RenderWindow   *mRenderWindow;
      InputManager         *mInputMgr;

      GameState            *mLoadState;
      GameState            *mPlayState;

      bool                 bShutdown;
      static GameManager   *mGameManager;
      unsigned short mWaitTime;

      std::vector<GameState*> mStates;

      GameManager(void);
      GameManager(const GameManager&) { }
      GameManager & operator = (const GameManager&);

      bool configureGame(void);
      void setupResources(void);

      bool keyPressed(const OIS::KeyEvent &e);
      bool keyReleased(const OIS::KeyEvent &e);

      bool mouseMoved(const OIS::MouseEvent &e);
      bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
      bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
};

}
#endif


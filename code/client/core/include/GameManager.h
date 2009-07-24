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
#include "OgreLogListener.h"
#include "GameSettings.h"
#include "GameCore.h"

namespace Client
{

class GameState;

/** The Game Manager
        @remarks
            The Client::GameManager class is our root class which deals with the
            initialisation and running of our game.
    */
class GameManager : public OIS::KeyListener, OIS::MouseListener
{
   public:
      bool                mSinglePlayer;

      ~GameManager(void);
      /** The method initialises Ogre::Root and other core parts of the game 
         such as MyGUI, Client::InputManager , Client::Console and 
         Client::GameSettings
         @param gameState
            The state used in this function will be the first state the game
            goes to after initialisation.
         @remarks
            This method should only be called once.
      */
      void startGame(GameState *gameState);
      /** This method changes the current GameState being run.
         @param gameState
            The GameState being changed to.
      */
      void changeState(GameState *gameState);
      void pushState(GameState *gameState);
      void popState(void);
      /** Shuts down the game */
      void requestShutdown(void);

      static GameManager* getSingletonPtr(void);
      Network*  getNetwork(void);
      MyGUI::Gui           *mGUI;
      Console              *mConsole;

      Ogre::Camera         *mCamera;
      Ogre::SceneManager   *mSceneMgr;
      Ogre::Viewport       *mViewport;

   private:
      Ogre::Root           *mRoot;
      Ogre::RenderWindow   *mRenderWindow;
      InputManager         *mInputMgr;
      GameSettings         *mSettings;
      GameCore             *mGameCore;
      GameState            *mLoadState;
      GameState            *mPlayState;
      Network              *mNetwork;

      OgreLogListener      *mOgreLogListener;

      bool                 bShutdown;
      static GameManager   *mGameManager;

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


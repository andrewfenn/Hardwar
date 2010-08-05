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

#include <OgreRoot.h>

#include "Game.h"
#include "GameState.h"
#include "srvstructs.h"
#include "BuildEditor.h"
#include "Network.h"

namespace Client
{

class GameManager;
class BuildEditor;

class PlayState : public GameState
{
   public:
      PlayState();
      ~PlayState();

      void enter();
      void exit();

      void pause();
      void resume();
      void update(unsigned long lTimeElapsed);
      void redraw();

      void keyPressed(const OIS::KeyEvent &e);
      void keyReleased(const OIS::KeyEvent &e);


      void mouseMoved(const OIS::MouseEvent &e);
      void mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
      void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
   private:
      Ogre::Root              *mRoot;
      Ogre::Camera            *mCamera;
      Ogre::SceneManager      *mSceneMgr;
      Ogre::Viewport          *mViewport;
      Ogre::OverlayManager    *mOverlayMgr;
      Ogre::RenderWindow      *mWindow;

      OIS::Keyboard           *mInputDevice;
      MyGUI::Gui              *mGUI;

      GameManager             *mGameMgr;

      Ogre::SceneNode         *mWorldNode;
      Ogre::SceneNode         *mWaterNode;

      BuildEditor             *mBuildEditor;
      Ogre::Degree            mMouseRotX, mMouseRotY;
      int                     mKeydownUp, mKeydownDown, 
                              mKeydownRight, mKeydownLeft;
      Ogre::Real              fpstimer;
};

}


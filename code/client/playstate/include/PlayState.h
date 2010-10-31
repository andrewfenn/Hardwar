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

#include "GuiTask.h"
#include "GameState.h"
#include "srvstructs.h"
#include "NetworkTask.h"

namespace Client
{
class PlayState : public GameState
{
   public:
      PlayState() : GameState("playstate") { }
      void enter();
      void exit();

      void update(unsigned long lTimeElapsed);

      void keyPressed(const OIS::KeyEvent &e);
      void keyReleased(const OIS::KeyEvent &e);

      void mouseMoved(const OIS::MouseEvent &e);
      void mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
      void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
   private:
      Ogre::OverlayManager    *mOverlayMgr;
      Ogre::RenderWindow      *mWindow;

      OIS::Keyboard           *mInputDevice;
      MyGUI::Gui              *mGUI;

      Ogre::SceneNode         *mWorldNode;
      Ogre::SceneNode         *mWaterNode;

      Ogre::Degree            mMouseRotX, mMouseRotY;
      int                     mKeydownUp, mKeydownDown, 
                              mKeydownRight, mKeydownLeft;
      Ogre::Real              fpstimer;
};

}


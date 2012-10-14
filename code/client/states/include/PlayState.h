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

#pragma once

#include <OgreRoot.h>
#include <OIS.h>

#include "GameState.h"

namespace Client
{
class PlayState : public GameState, OIS::KeyListener, OIS::MouseListener
{
   public:
      PlayState() : GameState("playstate") { }
      ~PlayState();
      void enter();

      void update(unsigned long timeElapsed);

      bool keyPressed(const OIS::KeyEvent &e);
      bool keyReleased(const OIS::KeyEvent &e);

      bool mouseMoved(const OIS::MouseEvent &e);
      bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
      bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
   private:
      Ogre::OverlayManager    *mOverlayMgr;
      Ogre::RenderWindow      *mWindow;

      OIS::Keyboard           *mInputDevice;

      Ogre::SceneNode         *mWorldNode;
      Ogre::SceneNode         *mWaterNode;

      Ogre::Degree            mMouseRotX, mMouseRotY;
      int                     mKeydownUp, mKeydownDown, 
                              mKeydownRight, mKeydownLeft;
      Ogre::Real              fpstimer;
};

}


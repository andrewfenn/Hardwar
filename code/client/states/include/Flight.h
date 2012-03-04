/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2011  Andrew Fenn
    
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

#include <OIS.h>
#include <Ogre.h>
#include "GameState.h"
#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>

namespace Client
{
class Flight : public GameState, OIS::KeyListener, OIS::MouseListener
{
   public:
      Flight() : GameState("flightstate") { }
      ~Flight();
      void enter();
      void update(unsigned long timeElapsed);
      
      bool keyPressed(const OIS::KeyEvent &e);
      bool keyReleased(const OIS::KeyEvent &e);

      bool mouseMoved(const OIS::MouseEvent &e);
      bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
      bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
   private:
      int  mKeyThrustUp, mKeyThrustDown;
      int  mKeydownUp, mKeydownDown, mKeydownRight, mKeydownLeft;
      Ogre::Degree  mMouseRotX, mMouseRotY;

      int mThrottle, mThrottleLimit, mThrust, mAcceleration, mSpeed, mMass, mSpeedLimit;

      void updateStats();
      Ogre::Overlay* mDebugOverlay;
};

}


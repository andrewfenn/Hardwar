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
#include <OISKeyboard.h>
#include <OISMouse.h>

namespace Client
{
class RootGameState : public GameState, OIS::KeyListener, OIS::MouseListener
{
public:
   RootGameState(GameTaskList* gametasks, Ogre::Root* root, Ogre::Viewport* viewport);
   ~RootGameState();
   void update(unsigned long lTimeElapsed);
   const bool shouldExit();
   void shutdown();

   bool keyPressed(const OIS::KeyEvent &e);
   bool keyReleased(const OIS::KeyEvent &e);
   bool mouseMoved(const OIS::MouseEvent &e );
   bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
   bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
private:
   bool mShutdown;
};
}


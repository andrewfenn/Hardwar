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
#include <OIS.h>

#include "GameTask.h"

namespace Client
{
class GameState;
typedef std::map<Ogre::String, GameState*> GameStateList;

class GameState {
public:
   GameState(const Ogre::String&);

   virtual void enter() {}
   virtual void exit() {}

   virtual void update(unsigned long lTimeElapsed);

   virtual void keyPressed(const OIS::KeyEvent &e);
   virtual void keyReleased(const OIS::KeyEvent &e);

   virtual void mouseMoved(const OIS::MouseEvent &e);
   virtual void mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
   virtual void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

   /** Internal only method */
   void setParent(GameState*);

   const Ogre::String getName();
   void shutdown();

   GameState* add(GameState*);
   GameState* get(const Ogre::String&);
   bool has(const Ogre::String&);
   void remove(const Ogre::String&);
   void pause();
   void resume();
protected:
   GameTaskList* mTasklist;
   bool mPaused;
   GameState* mParent;
   GameStateList mChildren;
   Ogre::String mName;
};

class RootGameState : public GameState
{
public:
   RootGameState() : GameState("Root") { }

   void update(unsigned long lTimeElapsed)
   {
      
   }

   void setTaskList(GameTaskList* gametasks)
   {
      mTasklist = gametasks;
   }
};
}


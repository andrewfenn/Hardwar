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
   virtual void update(unsigned long timeElapsed);

   /** Internal only method for setting up the data in the state propperly */
   void setParent(GameState*, GameTaskList*, Ogre::Root*, Ogre::Viewport*);
   /** Gets the name of the current gamestate */
   const Ogre::String getName();
   /** Gets the parent gamestate */
   void* getParent();
   void shutdown();
   /** Adds a gamestate to another gamestate's child list */
   void* add(GameState*);
   /** retrieves a gamestate based upon it's name. */
   void* get(const Ogre::String&);
   /** Replaces the current gamestate with a different one. */
   void replace(GameState*);
   /** Checks to see if any chilren exist in the list with the defined name
   given. */
   bool has(const Ogre::String&);
   /** Checks to see if any chilren exist in the list with the defined gamestate
   given. */
   bool has(GameState&);
   /** Removes a child from the parents game state list */
   void remove(const Ogre::String&);
   /** For times when you wish to remove the instance you are in. Will remove
   the state from its parent upon the next update assuming you are calling
   updateInternals in your update function */
   void markRemoval();
   /** Used to remove all children being held by the parent */
   void removeAllChildren();
   /** Prevents the child and it's children from running however maybe
   overrided by the gamestate that does not respect the mPaused variable */
   void pause();
   /** Allows the child and it's children to update again. */
   void resume();
   /** Used by parent to determine if the child wishes to be removed */
   bool shouldRemove();
   /** Updates all children set to this gamestate */
   void updateAllChildren(unsigned long timeElapsed);
protected:
   bool mPaused;
   bool mMarkedRemoved;
   GameState* mParent;
   GameStateList mChildren;
   Ogre::String mName;
   Ogre::Root           *mRoot;
   Ogre::SceneManager   *mSceneMgr;
   Ogre::Camera         *mCamera;
   Ogre::Viewport       *mViewport;
   GameTaskList         *mTasklist;
};
}


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

#include "GameState.h"
#include "GameTask.h"

using namespace Client;

GameState::GameState(const Ogre::String& name)
{
   mName = name;
   mPaused = false;
}

void GameState::setParent(GameState* parent)
{
   mParent = parent;
   mTasklist = parent->mTasklist;
}

void GameState::shutdown()
{
   this->exit();

   for (GameStateList::iterator i=mChildren.begin(); i != mChildren.end(); i++)
   {
      i->second->shutdown();
      mChildren.erase(i);
   }

   if (mParent)
   {
      mParent->remove(this->getName());
   }
}

const Ogre::String GameState::getName()
{
   return mName;
}

GameState* GameState::add(GameState* state)
{
   if (mChildren.find(state->getName()) != mChildren.end())
   {
      OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
         "A state with the name " + state->getName() + " already exists",
         "GameManager::createState" );
   }
   state->setParent(this);
   mChildren.insert(GameStateList::value_type(state->getName(), state));
   return state;
}

GameState* GameState::get(const Ogre::String& name)
{
   GameStateList::const_iterator i = mChildren.find(name);
   if (i != mChildren.end())
   {
      return (GameState*) &i->second;
   }
   OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
         "Cannot find State with name " + name,
         "GameTask::getState");
}

bool GameState::has(const Ogre::String& name)
{
   return (mChildren.find(name) != mChildren.end());
}

void GameState::remove(const Ogre::String& name)
{
   GameStateList::iterator i = mChildren.find(name);
   if (i != mChildren.end())
   {
      OGRE_DELETE &i->second;
      mChildren.erase(i);
   }
}

void GameState::pause()
{
   mPaused = true;
}

void GameState::resume()
{
   mPaused = false;
}

void GameState::update(unsigned long lTimeElapsed)
{
   if (mPaused)
      return;

   for (GameStateList::iterator i=mChildren.begin(); i != mChildren.end(); i++)
   {
      i->second->update(lTimeElapsed);
   }
}

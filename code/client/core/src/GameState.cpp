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
#include "RootGameState.h"

using namespace Client;

GameState::GameState(const Ogre::String& name)
{
   mName = name;
   mPaused = false;
   mParent = 0;
   mMarkedRemoved = false;
}

GameState::~GameState()
{
   this->removeAllChildren();
}

void GameState::setParent(GameState* parent, GameTaskList* gametasks, 
                                     Ogre::Root* root, Ogre::Viewport* viewport)
{
    mParent   = parent;
    mTasklist = gametasks;
    mRoot     = root;
    mSceneMgr = root->getSceneManager("GameSceneMgr");
    mCamera   = mSceneMgr->getCamera("GameCamera");
    mViewport = viewport;
}

void GameState::shutdown()
{
   this->removeAllChildren();
   if (mParent)
   {
      mParent->shutdown();
   } else {
      ((RootGameState*)this)->shutdown();
   }
}

const Ogre::String GameState::getName()
{
   return mName;
}

void* GameState::add(GameState* state)
{
   if (mChildren.find(state->getName()) != mChildren.end())
   {
      OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
         "A state with the name " + state->getName() + " already exists",
         "GameManager::add" );
   }
   state->setParent(this, mTasklist, mRoot, mViewport);
   mChildren.insert(GameStateList::value_type(state->getName(), state));
   state->enter();
   return state;
}

void* GameState::get(const Ogre::String& name)
{
   GameStateList::const_iterator i = mChildren.find(name);
   if (i != mChildren.end())
   {
      return (GameState*) &i->second;
   }
   OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
         "Cannot find State with name " + name,
         "GameTask::get");
}

void GameState::replace(GameState* state)
{
   mParent->add(state);
   this->markRemoval();
}

void GameState::markRemoval()
{
   mMarkedRemoved = true;
}

bool GameState::shouldRemove()
{
   return mMarkedRemoved;
}

bool GameState::has(const Ogre::String& name)
{
   return (mChildren.find(name) != mChildren.end());
}

bool GameState::has(GameState& state)
{
   return (mChildren.find(state.getName()) != mChildren.end());
}

void* GameState::getParent()
{
   return mParent;
}

void GameState::remove(const Ogre::String& name)
{
   GameStateList::iterator i = mChildren.find(name);
   if (i != mChildren.end())
   {
      OGRE_DELETE i->second;
      mChildren.erase(i);
   }
}

void GameState::removeAllChildren()
{
   GameStateList::iterator i;
   for (i = mChildren.begin(); i != mChildren.end(); i++)
   {
      OGRE_DELETE i->second;
   }
   mChildren.clear();
}

void GameState::updateAllChildren(unsigned long timeElapsed)
{
   GameStateList::iterator i;
   for (i = mChildren.begin(); i != mChildren.end(); i++)
   {
      if (i->second->shouldRemove())
      {
         this->remove(i->second->getName());
      }
      else
      {
         if (mPaused)
         {
            continue;
         }
         i->second->update(timeElapsed);
      }
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

void GameState::update(unsigned long timeElapsed)
{
   this->updateAllChildren(timeElapsed);
}

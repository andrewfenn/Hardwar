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

#include "RootGameState.h"

using namespace Client;

RootGameState::RootGameState(GameTaskList* gametasks, Ogre::Root* root, Ogre::Viewport* viewport) : GameState("Root")
{
   mTasklist = gametasks;
   mRoot     = root;
   mSceneMgr = root->getSceneManager("GameSceneMgr");
   mCamera   = mSceneMgr->getCamera("GameCamera");
   mViewport = viewport;
   mShutdown = false;
}

RootGameState::~RootGameState()
{
}

void RootGameState::update(unsigned long timeElapsed)
{
   this->updateAllChildren(timeElapsed);
}

const bool RootGameState::shouldExit()
{
   return mShutdown;
}

void RootGameState::shutdown()
{
   this->removeAllChildren();
   mShutdown = true;
}

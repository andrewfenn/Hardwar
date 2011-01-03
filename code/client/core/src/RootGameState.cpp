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
#include "InputTask.h"
#include "GuiTask.h"
#include <MyGUI.h>

using namespace Client;

RootGameState::RootGameState(GameTaskList* gametasks, Ogre::Root* root, Ogre::Viewport* viewport) : GameState("Root")
{
   mTasklist = gametasks;
   mRoot     = root;
   mSceneMgr = root->getSceneManager("GameSceneMgr");
   mCamera   = mSceneMgr->getCamera("GameCamera");
   mViewport = viewport;
   mShutdown = false;

   // register input listener for console
   InputTask* input = (InputTask*) mTasklist->get("Input");
   input->addKeyListener(this, this->getName());
   input->addMouseListener(this, this->getName());
}

RootGameState::~RootGameState()
{
   InputTask* input = (InputTask*) mTasklist->get("Input");
   input->removeKeyListener(this->getName());
   input->removeMouseListener(this->getName());
}

void RootGameState::update(unsigned long lTimeElapsed)
{
   this->updateAllChildren(lTimeElapsed);
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

bool RootGameState::keyPressed(const OIS::KeyEvent &e)
{
   return true;
}

bool RootGameState::keyReleased(const OIS::KeyEvent &e)
{
   if (e.key == OIS::KC_GRAVE)
   {
/*      GuiTask* gui = (GuiTask*) mTasklist->get("Gui");
      gui->mGUI->setVisiblePointer(!gui->mGUI->isVisiblePointer());*/
      mRoot->getAutoCreatedWindow()->writeContentsToTimestampedFile("./screenshots/",".png");
   }
   return true;
}

bool RootGameState::mouseMoved(const OIS::MouseEvent &e)
{
   ((GuiTask*) mTasklist->get("Gui"))->resource()->injectMouseMove(e.state.X.abs, e.state.Y.abs, e.state.Z.abs);
   return true;
}

bool RootGameState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{ 
   return true;
}

bool RootGameState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{ 
   return true;
}

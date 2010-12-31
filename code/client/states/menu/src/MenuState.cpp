/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2010  Andrew Fenn
    
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

#include "MenuState.h"
#include "InputTask.h"
#include "GuiTask.h"
#include <MyGUI.h>

using namespace Client;

MenuState::MenuState() : GameState("menustate"){}

MenuState::~MenuState()
{
   InputTask* input = (InputTask*) mTasklist->get("Input");
   input->removeKeyListener(this->getName());
   input->removeMouseListener(this->getName());
   OGRE_DELETE mFade;
}

void MenuState::enter()
{
   mSceneMgr->clearScene();
   InputTask* input = (InputTask*) mTasklist->get("Input");
   input->addKeyListener(this, this->getName());
   input->addMouseListener(this, this->getName());
   mSceneMgr->setSkyBox(true, "Menu/MedResSkyBox", 5000);
   mFade = OGRE_NEW Fader("Menu/FadeInOut", "Materials/OverlayMaterial", this);
   mFade->startFadeIn(2);
   mStarted = false;
}

void MenuState::update(const unsigned long lTimeElapsed )
{
   if (mStarted)
   {
      // skip first frame to get accurate elapsed time
      mFade->fade(lTimeElapsed);
   }
   mStarted = true;
}

bool MenuState::keyPressed(const OIS::KeyEvent &e)
{ 
   return true;
}

bool MenuState::keyReleased(const OIS::KeyEvent &e)
{
   if( e.key == OIS::KC_ESCAPE )
   {
      /* FIXME: Should go to main menu */
      this->shutdown();
   }
   return true;
}

bool MenuState::mouseMoved(const OIS::MouseEvent &e)
{
   ((GuiTask*) mTasklist->get("Gui"))->mGUI->injectMouseMove(e.state.X.abs, e.state.Y.abs, e.state.Z.abs);
   return true;
}

bool MenuState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{ 
   return true;
}

bool MenuState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{ 
   return true;
}
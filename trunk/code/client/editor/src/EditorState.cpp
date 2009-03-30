/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright (C) 2008  Andrew Fenn
    
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

#include "EditorState.h"

Editorstate* Editorstate::mEditorstate;

void Editorstate::enter(void)
{
   mRoot         = Ogre::Root::getSingletonPtr();
   mOverlayMgr   = Ogre::OverlayManager::getSingletonPtr();
   mWindow       = mRoot->getAutoCreatedWindow();
   mSceneMgr     = mRoot->getSceneManager("PlaySceneMgr");
   mViewport     = mWindow->addViewport(mCamera, 0);
   mGameMgr      = GameManager::getSingletonPtr();
}

void Editorstate::exit(void)
{

}

void Editorstate::pause(void)
{
}

void Editorstate::resume(void)
{
}

void Editorstate::update(unsigned long lTimeElapsed)
{

   Ogre::Vector3 translateVector = Ogre::Vector3::ZERO;
   float scale = 5.0f;

   if (mKeydownUp == 1)
     translateVector.z -= scale;

   if (mKeydownDown == 1)
     translateVector.z += scale;

   if (mKeydownLeft == 1)
     translateVector.x -= scale;

   if (mKeydownRight == 1)
     translateVector.x += scale;

   mCamera->yaw(mMouseRotX);
   mCamera->pitch(mMouseRotY);    
   translateVector = mCamera->getOrientation() * translateVector;
   mCamera->setPosition(mCamera->getPosition()+translateVector);

   mMouseRotX = mMouseRotY = 0;
}

void Editorstate::keyPressed(const OIS::KeyEvent &e)
{
   if (e.key == OIS::KC_GRAVE)
   {}

   if (e.key == OIS::KC_W)
      mKeydownUp = 1;

   if (e.key == OIS::KC_S)
      mKeydownDown = 1;

   if (e.key == OIS::KC_A)
      mKeydownLeft = 1;

   if (e.key == OIS::KC_D)
      mKeydownRight = 1;

   if(e.key == OIS::KC_B)
      mSceneMgr->showBoundingBoxes(true);
}

void Editorstate::keyReleased(const OIS::KeyEvent &e)
{
   if (e.key == OIS::KC_ESCAPE)
      this->requestShutdown();

   if (e.key == OIS::KC_W)
      mKeydownUp = 0;

   if (e.key == OIS::KC_S)
      mKeydownDown = 0;

   if (e.key == OIS::KC_A)
      mKeydownLeft = 0;

   if (e.key == OIS::KC_D)
      mKeydownRight = 0;
}

void Editorstate::mouseMoved(const OIS::MouseEvent &e)
{
   const OIS::MouseState &mouseState = e.state;
   mMouseRotX = Ogre::Degree(-mouseState.X.rel * 0.13);
   mMouseRotY = Ogre::Degree(-mouseState.Y.rel * 0.13);
}

void Editorstate::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{ }

void Editorstate::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{ }

Editorstate* Editorstate::getSingletonPtr(void)
{
   if(!mEditorstate)
   {
      mEditorstate = new Editorstate();
   }
   return mEditorstate;
}


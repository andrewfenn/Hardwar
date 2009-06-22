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

#include "PlayState.h"

using namespace Client;

PlayState* PlayState::mPlayState;

void PlayState::enter(void)
{
   mGameMgr      = GameManager::getSingletonPtr();
   mRoot         = Ogre::Root::getSingletonPtr();
   mOverlayMgr   = Ogre::OverlayManager::getSingletonPtr();
   mWindow       = mRoot->getAutoCreatedWindow();
   mSceneMgr     = mRoot->getSceneManager("GameSceneMgr");
   mGUI          = MyGUI::Gui::getInstancePtr();
   mCamera       = mGameMgr->mCamera;
   mViewport     = mGameMgr->mViewport;


   mSceneMgr->setAmbientLight(Ogre::ColourValue::White);

   /* Setup our camera position in the world, we use the following coords
    * below because it is inside the model
    */
   mCamera->setPosition(Ogre::Vector3(43027.3, 1414.33, -30462.4));
   mCamera->setNearClipDistance(5);

   //mWorldNode = mSceneMgr->getRootSceneNode()->getSceneNode("world");

   mMouseRotX = mMouseRotY = 0;
   mKeydownUp = mKeydownDown = mKeydownRight = mKeydownLeft = 0;

   mBuildEditor = new BuildEditor;
}

void PlayState::exit(void)
{
   Network::getSingletonPtr()->stopThread();
   delete mBuildEditor;
}

/*void PlayState::switchToEditor(vector<String>& Params)
{
    PlayState* play = PlayState::getSingletonPtr(); 
  //  play->changeState( EditorState::getSingletonPtr());
}*/

void PlayState::pause(void)
{
}

void PlayState::resume(void)
{
}

void PlayState::update(unsigned long lTimeElapsed)
{
   mBuildEditor->update(lTimeElapsed);
   Ogre::Vector3 translateVector = Ogre::Vector3::ZERO;
   float scale = 0.9f;

   if (mKeydownUp == 1)
     translateVector.z -= scale*lTimeElapsed;

   if (mKeydownDown == 1)
     translateVector.z += scale*lTimeElapsed;

   if (mKeydownLeft == 1)
     translateVector.x -= scale*lTimeElapsed;

   if (mKeydownRight == 1)
     translateVector.x += scale*lTimeElapsed;

   mCamera->yaw(mMouseRotX);
   mCamera->pitch(mMouseRotY);    
   translateVector = mCamera->getOrientation() * translateVector;
   mCamera->setPosition(mCamera->getPosition()+translateVector);

   mMouseRotX = mMouseRotY = 0;
}

void PlayState::keyPressed(const OIS::KeyEvent &e)
{
   if (mGameMgr->mConsole->isVisible() && e.key != OIS::KC_GRAVE)
   {
      mGUI->injectKeyPress(e);
   }
   else
   {
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
}

void PlayState::keyReleased(const OIS::KeyEvent &e)
{
   if (e.key == OIS::KC_GRAVE)
      mGameMgr->mConsole->toggleShow();

   if (mGameMgr->mConsole->isVisible())
   {
      if (e.key == OIS::KC_ESCAPE)
         mGameMgr->mConsole->toggleShow();

      mGUI->injectKeyRelease(e);
   }
   else
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
}

void PlayState::mouseMoved(const OIS::MouseEvent &e)
{
   if (mGUI->isShowPointer() || mBuildEditor->isVisible())
   {
      /* fixme: should move this to game manager */
      mGUI->injectMouseMove(e);
   }
   else
   {
      const OIS::MouseState &mouseState = e.state;
      mMouseRotX = Ogre::Degree(-mouseState.X.rel * 0.13);
      mMouseRotY = Ogre::Degree(-mouseState.Y.rel * 0.13);
   }
}

void PlayState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   if (mGUI->isShowPointer() || mBuildEditor->isVisible())
   {
      mGUI->injectMousePress(e, id);
   }
}

void PlayState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   if (mGUI->isShowPointer() || mBuildEditor->isVisible())
   {
      mGUI->injectMouseRelease(e, id);
   }
}

PlayState* PlayState::getSingletonPtr(void)
{
   if(!mPlayState)
   {
      mPlayState = new PlayState();
   }
   return mPlayState;
}


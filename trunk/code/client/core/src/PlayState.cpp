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
   mOgreMax      = new OgreMax::OgreMaxScene;

   mSceneMgr->setAmbientLight(Ogre::ColourValue::White);

   /* Setup our camera position in the world, we use the following coords
    * below because it is inside the model
    */
   mCamera->setPosition(Ogre::Vector3(43027.3, 1414.33, -30462.4));
   mCamera->setNearClipDistance(5);

   mWorldNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("world");
   mWorldNode->attachObject(mCamera);

   /* load world */
   const Ogre::String filename = Ogre::String("../media/hardwar/non-free/world.scene");
   mOgreMax->Load(filename, mWindow, 0, mSceneMgr, mWorldNode);

   mMouseRotX = mMouseRotY = 0;
   mKeydownUp = mKeydownDown = mKeydownRight = mKeydownLeft = 0;

   mPingWaitTime = 0;
   mPingSent = false;
   mPingTime = 0;
   mShowFPS = false;

   mGameMgr->mConsole->addCommand(Ogre::UTFString("cl_showfps"), MyGUI::newDelegate(this, &PlayState::cmd_showFPS));
}

void PlayState::exit(void)
{

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

void PlayState::cmd_showFPS(const Ogre::UTFString &key, const Ogre::UTFString &value)
{
   bool show;
   if (!MyGUI::utility::parseComplex(value, show))
   {
      if (!value.empty())
      {
         mGameMgr->mConsole->addToConsole(mGameMgr->mConsole->getConsoleError(), key, value);
      }
      mGameMgr->mConsole->addToConsole(mGameMgr->mConsole->getConsoleFormat(), key, "[true|false] - "+Ogre::UTFString("Show debug information"));
   }
   else
   {
      mGameMgr->mConsole->addToConsole(mGameMgr->mConsole->getConsoleSuccess(), key, value);
      mShowFPS = show;
      if (!show)
      {
         /* hide the overlay */
         Ogre::Overlay* o = Ogre::OverlayManager::getSingleton().getByName("Core/DebugOverlay");
         o->hide();
      }
   }
}

void PlayState::update(unsigned long lTimeElapsed)
{
   networkUpdate(lTimeElapsed);
   fpstimer += lTimeElapsed;
   if (fpstimer > 0.5)
   {
      /* Don't need to update this stuff every frame */
      fpstimer = 0;
      if (mShowFPS) showFPS();
   }

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

void PlayState::networkUpdate(unsigned long lTimeElapsed)
{
   mPingWaitTime += lTimeElapsed;
   if (mPingWaitTime > 3000)
   {
      if (mPingSent)
      {
         /* No reply, send again */
         mPingSent = false;
      }
      else
      {
         /* Wait before sending another ping request */
         if (mGameMgr->mNetwork->message("ping", strlen("ping")+1, 0, ENET_PACKET_FLAG_UNSEQUENCED))
         {
             mPingSent = true;
         }
      }
      mPingWaitTime = 0;
   }

   ENetEvent lEvent;
   mGameMgr->mNetwork->pollMessage(&lEvent);

   switch(lEvent.type)
   {
      case ENET_EVENT_TYPE_RECEIVE:
         switch(lEvent.channelID)
         {
            case 0:
            {
               char* data = (char*)lEvent.packet->data;

               if (strcmp(data, "pong") == 0)
               {
                  mPingTime = mPingWaitTime;
               }
            }
            break;
            default:
            break;
         }
      break;
      default:
      break;
   }
}

void PlayState::showFPS(void)
{
   static Ogre::String currFps = "Current FPS: ";
   static Ogre::String avgFps = "Average FPS: ";
   static Ogre::String worstFps = "X: ";
   static Ogre::String bestFps = "Y: ";
   static Ogre::String tris = "Z: ";

   /* update stats when necessary */
   Ogre::OverlayElement* guiAvg   = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
   Ogre::OverlayElement* guiCurr  = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
   Ogre::OverlayElement* guiBest  = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");
   Ogre::OverlayElement* guiWorst = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
   Ogre::OverlayElement* guiTris  = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
   Ogre::OverlayElement* guiBatch = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
   Ogre::OverlayElement* guiImage = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/LogoPanel");
   guiImage->hide();

   guiAvg->setCaption(avgFps + Ogre::StringConverter::toString(mWindow->getAverageFPS()));
   guiCurr->setCaption(currFps + Ogre::StringConverter::toString(mWindow->getLastFPS()));
   guiBest->setCaption(bestFps + Ogre::StringConverter::toString(mCamera->getPosition().y));
   guiWorst->setCaption(worstFps + Ogre::StringConverter::toString(mCamera->getPosition().x));
   guiTris->setCaption(tris + Ogre::StringConverter::toString(mCamera->getPosition().z));
   guiBatch->setCaption("Ping: " + Ogre::StringConverter::toString(mPingTime));
   Ogre::Overlay* o = Ogre::OverlayManager::getSingleton().getByName("Core/DebugOverlay");

   if (!o)
   {
      OGRE_EXCEPT( Ogre::Exception::ERR_ITEM_NOT_FOUND, "Could not find overlay Core/DebugOverlay", "showDebugOverlay" );
   }
   else
   {
      o->show();
   }
}

void PlayState::keyPressed(const OIS::KeyEvent &e)
{
   if (mGameMgr->mConsole->isVisible())
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
   if (mGameMgr->mConsole->isVisible())
   {
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
   if (mGameMgr->mConsole->isVisible())
   {
      mGUI->injectMousePress(e, id);
   }
}

void PlayState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   if (mGameMgr->mConsole->isVisible())
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


/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2011  Andrew Fenn
    
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

#include "Flight.h"
#include "InputTask.h"

#include <math.h>

using namespace Client;

Flight::~Flight()
{
   InputTask* input = (InputTask*) mTasklist->get("Input");
   input->removeKeyListener(this->getName());
   input->removeMouseListener(this->getName());
}

void Flight::enter()
{
   InputTask* input = (InputTask*) mTasklist->get("Input");
   input->addKeyListener(this, this->getName());
   input->addMouseListener(this, this->getName());
   mKeydownUp = mKeydownDown = mKeydownRight = mKeydownLeft = 0;
   mThrottleLimit = 100;
   mThrottle = 0;
   mMass = 100;
   mAcceleration = 3;
   mSpeed = 0;
   mSpeedLimit = 560;

   mDebugOverlay = Ogre::OverlayManager::getSingleton().getByName("Core/DebugOverlay");
   mDebugOverlay->show();
}

void Flight::update(unsigned long timeElapsed)
{
   Ogre::Vector3 translateVector = Ogre::Vector3::ZERO;
/*
   TODO: Put this in a free cam mode
   if (mKeydownUp == 1)
     translateVector.z -= scale*timeElapsed;

   if (mKeydownDown == 1)
     translateVector.z += scale*timeElapsed;

   if (mKeydownLeft == 1)
     translateVector.x -= scale*timeElapsed;

   if (mKeydownRight == 1)
     translateVector.x += scale*timeElapsed;

   mCamera->yaw(mMouseRotX);
   mCamera->pitch(mMouseRotY);    
   translateVector = mCamera->getOrientation() * translateVector;
   mCamera->setPosition(mCamera->getPosition()+translateVector);
   mMouseRotX = mMouseRotY = 0;
   */
   
   if (mKeyThrustDown && mThrottle > mThrottleLimit*-1)
      mThrottle--;

   if (mKeyThrustUp && mThrottle < mThrottleLimit)
      mThrottle++;

   if (mSpeed > 0)
      mAcceleration = 1 + mSpeed * 0.001;
   else
      mAcceleration = 1 + (mSpeed*-1) * 0.001;

   mThrust = ceil(mMass*(((float)mAcceleration/1000)*mThrottle));
   mSpeed += mThrust;
   
   if (mSpeed > mSpeedLimit)
      mSpeed = mSpeedLimit;

   if (mSpeed < (mSpeedLimit/2)*-1)
      mSpeed = (mSpeedLimit/2)*-1;
      
   // stupid simple drag
   mSpeed -= mSpeed*0.01;
   
   mCamera->yaw(mMouseRotX);
   mCamera->pitch(mMouseRotY);

   // divide speed by frames per second
   translateVector.z -= (float) mSpeed/60;
   translateVector = mCamera->getOrientation() * translateVector;
   mCamera->setPosition(mCamera->getPosition()+translateVector);

   mMouseRotX = mMouseRotY = 0;
   
   this->updateStats();
}


bool Flight::keyPressed(const OIS::KeyEvent &e)
{
   switch(e.key)
   {
      case OIS::KC_MINUS:
         mKeyThrustDown = 1;
      break;
      case OIS::KC_EQUALS:
         mKeyThrustUp = 1;
      break;
      case OIS::KC_UP:
         mKeydownUp = 1;
      break;
      case OIS::KC_DOWN:
         mKeydownDown = 1;
      break;
      case OIS::KC_LEFT:
         mKeydownLeft = 1;
      break;
      case OIS::KC_RIGHT:
         mKeydownRight = 1;
      break;
      default:
      break;
   }
         
   return true;
}

bool Flight::keyReleased(const OIS::KeyEvent &e)
{
   switch(e.key)
   {
      case OIS::KC_MINUS:
         mKeyThrustDown = 0;
      break;
      case OIS::KC_EQUALS:
         mKeyThrustUp = 0;
      break;
      case OIS::KC_UP:
         mKeydownUp = 0;
      break;
      case OIS::KC_DOWN:
         mKeydownDown = 0;
      break;
      case OIS::KC_LEFT:
         mKeydownLeft = 0;
      break;
      case OIS::KC_RIGHT:
         mKeydownRight = 0;
      break;
      case OIS::KC_RETURN:
         mThrottle = 0;
      break;
      case OIS::KC_0:
         mThrottle = 100;
      break;
      case OIS::KC_1:
         mThrottle = 10;
      break;
      case OIS::KC_2:
         mThrottle = 20;
      break;
      case OIS::KC_3:
         mThrottle = 30;
      break;
      case OIS::KC_4:
         mThrottle = 40;
      break;
      case OIS::KC_5:
         mThrottle = 50;
      break;
      case OIS::KC_6:
         mThrottle = 60;
      break;
      case OIS::KC_7:
         mThrottle = 70;
      break;
      case OIS::KC_8:
         mThrottle = 80;
      break;
      case OIS::KC_9:
         mThrottle = 90;
      break;
      case OIS::KC_RSHIFT:
         mThrottle = -100;
      break;
      case OIS::KC_BACK:
         mThrottle = 100;
      break;
      default:
      break;
   }

   return true;
}

bool Flight::mouseMoved(const OIS::MouseEvent &e)
{
   const OIS::MouseState &mouseState = e.state;
   mMouseRotX = Ogre::Degree(-mouseState.X.rel * 0.13);
   mMouseRotY = Ogre::Degree(-mouseState.Y.rel * 0.13);
   return true;
}

bool Flight::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   return true;
}

bool Flight::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   return true;
}

void Flight::updateStats(void)
{
	static Ogre::String currFps = "Throttle: ";
	static Ogre::String avgFps = "Speed: ";
	static Ogre::String bestFps = "Thrust: ";
	static Ogre::String worstFps = "";
	static Ogre::String tris = "";
	static Ogre::String batches = "";

	// update stats when necessary
	try {
		Ogre::OverlayElement* guiAvg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
		guiAvg->setCaption(avgFps+Ogre::StringConverter::toString(mSpeed));
		Ogre::OverlayElement* guiCurr = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
		guiCurr->setCaption(currFps+Ogre::StringConverter::toString(mThrottle));
		Ogre::OverlayElement* guiBest = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
		guiBest->setCaption(bestFps+Ogre::StringConverter::toString(mThrust));

		Ogre::OverlayElement* guiWorst = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");
		Ogre::OverlayElement* guiTris = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
		
		Ogre::OverlayElement* guiBatches = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
		Ogre::OverlayElement* guiDbg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
	}
	catch(...) { /* ignore */ }
}

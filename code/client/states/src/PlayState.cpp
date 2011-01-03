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

#include "PlayState.h"

#include "srvstructs.h"

#include "GuiTask.h"
#include "NetworkTask.h"

using namespace Client;

void PlayState::enter()
{
   mOverlayMgr   = Ogre::OverlayManager::getSingletonPtr();
   mWindow       = mRoot->getAutoCreatedWindow();
   mGUI          = MyGUI::Gui::getInstancePtr();

   mSceneMgr->setAmbientLight(Ogre::ColourValue::White);

   /* Setup our camera position in the world, we use the following coords
    * below because it is inside the model
    */
   mCamera->setPosition(Ogre::Vector3(43027.3, 1414.33, -30462.4));
   mCamera->setNearClipDistance( 0.2f );
   mCamera->setQueryFlags(0xFFFFFFFF);

   mMouseRotX = mMouseRotY = 0;
   mKeydownUp = mKeydownDown = mKeydownRight = mKeydownLeft = 0;

	// create a lightsource
	Ogre::Light* light = mSceneMgr->createLight( "Sun" );
	light->setType( Ogre::Light::LT_DIRECTIONAL );
	light->setDiffuseColour( Ogre::ColourValue( .82, .81, .7 ) );
	light->setSpecularColour( Ogre::ColourValue( .82, .81, .7 ) );
	light->setDirection( Ogre::Vector3( 0, -1, 1 ) ); 

}

void PlayState::exit()
{
   ((NetworkTask*)mTasklist->get("Network"))->stopThread();
}

void PlayState::update(unsigned long timeElapsed)
{
   Ogre::Vector3 translateVector = Ogre::Vector3::ZERO;
   float scale = 0.9f;

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
}

bool PlayState::keyPressed(const OIS::KeyEvent &e)
{
   if (e.key == OIS::KC_W)
      mKeydownUp = 1;

   if (e.key == OIS::KC_S)
      mKeydownDown = 1;

   if (e.key == OIS::KC_A)
      mKeydownLeft = 1;

   if (e.key == OIS::KC_D)
      mKeydownRight = 1;
         
   return true;
}

bool PlayState::keyReleased(const OIS::KeyEvent &e)
{
   if (e.key == OIS::KC_TAB)
   {
   }

   if (e.key == OIS::KC_ESCAPE)
      this->shutdown();

   if (e.key == OIS::KC_W)
      mKeydownUp = 0;

   if (e.key == OIS::KC_S)
      mKeydownDown = 0;

   if (e.key == OIS::KC_A)
      mKeydownLeft = 0;

   if (e.key == OIS::KC_D)
      mKeydownRight = 0;

   return true;
}

bool PlayState::mouseMoved(const OIS::MouseEvent &e)
{
   const OIS::MouseState &mouseState = e.state;
   mMouseRotX = Ogre::Degree(-mouseState.X.rel * 0.13);
   mMouseRotY = Ogre::Degree(-mouseState.Y.rel * 0.13);
   return true;
}

bool PlayState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   return true;
}

bool PlayState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   return true;
}

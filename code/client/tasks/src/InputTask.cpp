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

#include "InputTask.h"

#include <OISException.h>
#include <OISEvents.h>


using namespace Client;

InputTask::InputTask()
{
   mInputSystem = 0;
   mMouse = 0;
   mKeyboard = 0;
}

void InputTask::shutdown()
{
   if(mInputSystem)
   {
      /* Clear Listeners */
      mKeyListeners.clear();
      mMouseListeners.clear();
      mJoystickListeners.clear();

      if(mMouse)
      {
         mInputSystem->destroyInputObject( mMouse );
         mMouse = 0;
      }

      if(mKeyboard)
      {
         mInputSystem->destroyInputObject( mKeyboard );
         mKeyboard = 0;
      }

      if(mJoysticks.size() > 0)
      {
         itJoystick    = mJoysticks.begin();
         itJoystickEnd = mJoysticks.end();
         for(; itJoystick != itJoystickEnd; ++itJoystick)
         {
             mInputSystem->destroyInputObject(*itJoystick);
         }
         mJoysticks.clear();
      }

      mInputSystem->destroyInputSystem(mInputSystem);
      mInputSystem = 0;
   }
}

void InputTask::init()
{
   this->getJoystick(1);
   mRenderWindow = Ogre::Root::getSingletonPtr()->getAutoCreatedWindow();

   if( !mInputSystem )
   {
      /* Setup basic variables */
      OIS::ParamList paramList;
      size_t windowHnd = 0;
      std::ostringstream windowHndStr;
      unsigned long timeout = 0;

      /*
       * on windows OIS crashes because renderWindow hasn't initilized to make
       * sure this doesn't happen we wait for renderWindow to no longer be
       * null before continuing.
       */
      do
      {
         timeout++;
         /* Get window handle */
         mRenderWindow->getCustomAttribute( "WINDOW", &windowHnd );
      } while (windowHnd == 0 || timeout == 2000);

      if (windowHnd == 0)
      {
         OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,"windowHnd still null",
                                                                "initialise()");
      }

      /* Fill parameter list */
      windowHndStr << (unsigned int) windowHnd;
      paramList.insert(std::make_pair(std::string("WINDOW"),windowHndStr.str()));

      /* Create inputsystem */
      mInputSystem = OIS::InputManager::createInputSystem(paramList);
      /* If possible create a buffered keyboard */
      #if OIS_VERSION_MINOR >= 2
      if (mInputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0)
      #else
      if (mInputSystem->numKeyboards() > 0)
      #endif
      {
         mKeyboard = static_cast<OIS::Keyboard*>(mInputSystem->createInputObject(OIS::OISKeyboard, true));
         mKeyboard->setEventCallback(this);
      }

      /* If possible create a buffered mouse */
      #if OIS_VERSION_MINOR >= 2
      if (mInputSystem->getNumberOfDevices(OIS::OISMouse) > 0)
      #else
      if (mInputSystem->numMice() > 0)
      #endif
      {
         mMouse = static_cast<OIS::Mouse*>(mInputSystem->createInputObject(OIS::OISMouse, true));
         mMouse->setEventCallback(this);

         this->changeFocus(mRenderWindow);
      }

      /* If possible create all joysticks in buffered mode */
      #if OIS_VERSION_MINOR >= 2
      if (mInputSystem->getNumberOfDevices(OIS::OISJoyStick) > 0)
      #else
      if (mInputSystem->numJoySticks() > 0)
      #endif
      {
         #if OIS_VERSION_MINOR >= 2
         mJoysticks.resize(mInputSystem->getNumberOfDevices(OIS::OISJoyStick));
         #else
         mJoysticks.resize(mInputSystem->numJoySticks());
         #endif

         itJoystick    = mJoysticks.begin();
         itJoystickEnd = mJoysticks.end();
         for(; itJoystick != itJoystickEnd; ++itJoystick)
         {
             (*itJoystick) = static_cast<OIS::JoyStick*>( mInputSystem->createInputObject(OIS::OISJoyStick, true));
             (*itJoystick)->setEventCallback(this);
         }
      }
   }
}

void InputTask::update()
{
   /* Need to capture and update each device for every frame */
   if(mMouse)
   {
      mMouse->capture();
   }

   if(mKeyboard)
   {
      mKeyboard->capture();
   }

   if(mJoysticks.size() > 0)
   {
      itJoystick    = mJoysticks.begin();
      itJoystickEnd = mJoysticks.end();
      for(; itJoystick != itJoystickEnd; ++itJoystick)
      {
         (*itJoystick)->capture();
      }
   }
}

bool InputTask::disable(const std::string& instanceName)
{
   if (mDisabled.find(instanceName) != mDisabled.end())
   {
      mDisabled[instanceName]++;
      return true;
   }
   return false;
}

bool InputTask::enable(const std::string& instanceName)
{
   if (mDisabled.find(instanceName) != mDisabled.end())
   {
      if (mDisabled[instanceName] > 0)
      {
         mDisabled[instanceName]--;
         return true;
      }
   }
   return false;
}

void InputTask::disableAll()
{
   std::map<std::string, short>::iterator it;
   for(it = mDisabled.begin(); it != mDisabled.end(); ++it)
   {
      it->second++;
   }
}

void InputTask::enableAll()
{
   std::map<std::string, short>::iterator it;
   for(it = mDisabled.begin(); it != mDisabled.end(); ++it)
   {
      if (it->second > 0)
         it->second--;
   }
}

bool InputTask::isEnabled(const std::string& instanceName)
{
   if (mDisabled.find(instanceName) == mDisabled.end())
   {
      throw Ogre::Exception( Ogre::Exception::ERR_INTERNAL_ERROR, "Error - "+instanceName+" could not be found", "InputTask::isEnabled");
   }

   return mDisabled[instanceName] == 0;
}

void InputTask::addKeyListener(OIS::KeyListener *keyListener,
                                                const std::string& instanceName)
{
   if(mKeyboard)
   {
      /* Check for duplicate items */
      itKeyListener = mKeyListeners.find(instanceName);
      if(itKeyListener == mKeyListeners.end())
      {
         mKeyListeners[instanceName] = keyListener;
         if (mDisabled.find(instanceName) == mDisabled.end())
            mDisabled[instanceName] = 0;
      }
   }
}

void InputTask::addMouseListener(OIS::MouseListener *mouseListener,
                                                const std::string& instanceName)
{
   if(mMouse)
   {
      /* Check for duplicate items */
      itMouseListener = mMouseListeners.find(instanceName);
      if( itMouseListener == mMouseListeners.end() )
      {
         mMouseListeners[instanceName] = mouseListener;
         if (mDisabled.find(instanceName) == mDisabled.end())
            mDisabled[instanceName] = 0;
      }
   }
}

void InputTask::addJoystickListener(OIS::JoyStickListener *joystickListener,
                                                const std::string& instanceName)
{
   if(mJoysticks.size() > 0)
   {
      /* Check for duplicate items */
      itJoystickListener = mJoystickListeners.find(instanceName);
      if(itJoystickListener != mJoystickListeners.end())
      {
         mJoystickListeners[ instanceName ] = joystickListener;
         if (mDisabled.find(instanceName) == mDisabled.end())
            mDisabled[instanceName] = 0;
      }
   }
}

void InputTask::removeKeyListener(const std::string& instanceName)
{
   std::cout << instanceName << std::endl;
   itKeyListener = mKeyListeners.find(instanceName);
   if(itKeyListener != mKeyListeners.end())
   {
      mKeyListeners.erase(itKeyListener);
   }
}

void InputTask::removeMouseListener(const std::string& instanceName)
{
   itMouseListener = mMouseListeners.find(instanceName);
   if(itMouseListener != mMouseListeners.end())
   {
      mMouseListeners.erase(itMouseListener);
   }
}

void InputTask::removeJoystickListener(const std::string& instanceName)
{
   itJoystickListener = mJoystickListeners.find(instanceName);
   if(itJoystickListener != mJoystickListeners.end())
   {
      mJoystickListeners.erase(itJoystickListener);
   }
}

void InputTask::removeKeyListener(OIS::KeyListener *keyListener)
{
   itKeyListener       = mKeyListeners.begin();
   itKeyListenerEnd    = mKeyListeners.end();
   for(; itKeyListener != itKeyListenerEnd; ++itKeyListener)
   {
      if(itKeyListener->second == keyListener)
      {
         mKeyListeners.erase(itKeyListener);
         break;
      }
   }
}

void InputTask::removeMouseListener(OIS::MouseListener *mouseListener)
{
   itMouseListener         = mMouseListeners.begin();
   itMouseListenerEnd      = mMouseListeners.end();
   for(; itMouseListener  != itMouseListenerEnd; ++itMouseListener)
   {
      if(itMouseListener->second == mouseListener)
      {
         mMouseListeners.erase(itMouseListener);
         break;
      }
   }
}

void InputTask::removeJoystickListener(OIS::JoyStickListener *joystickListener)
{
   itJoystickListener    = mJoystickListeners.begin();
   itJoystickListenerEnd = mJoystickListeners.end();
   for(; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener)
   {
      if(itJoystickListener->second == joystickListener)
      {
         mJoystickListeners.erase(itJoystickListener);
         break;
      }
   }
}

void InputTask::removeAllListeners(void)
{
   mKeyListeners.clear();
   mMouseListeners.clear();
   mJoystickListeners.clear();
   mDisabled.clear();
}

void InputTask::removeAllKeyListeners(void)
{
   mKeyListeners.clear();
}

void InputTask::removeAllMouseListeners(void)
{
   mMouseListeners.clear();
}

void InputTask::removeAllJoystickListeners(void)
{
   mJoystickListeners.clear();
}

void InputTask::changeSize(Ogre::RenderWindow* rw)
{
   unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void InputTask::changeFocus(Ogre::RenderWindow* rw)
{
   if (rw->isFullScreen())
   {
      // TODO: enable mouse locking to window for linux
   }
   else
   {
      // TODO: disable mouse locking to window for linux
   }

   changeSize(rw);
}

OIS::Mouse* InputTask::getMouse(void)
{
   return mMouse;
}

OIS::Keyboard* InputTask::getKeyboard(void)
{
   return mKeyboard;
}

OIS::JoyStick* InputTask::getJoystick(unsigned int index)
{
   /* Make sure it's a valid index */
   if(index < mJoysticks.size())
   {
     return mJoysticks[index];
   }
   return 0;
}

int InputTask::getNumOfJoysticks(void)
{
   /* Cast to keep compiler happy */
   return (int) mJoysticks.size();
}

bool InputTask::keyPressed(const OIS::KeyEvent &e)
{
   itKeyListener    = mKeyListeners.begin();
   itKeyListenerEnd = mKeyListeners.end();
   for(;itKeyListener != itKeyListenerEnd; ++itKeyListener)
   {
      if (this->isEnabled(itKeyListener->first))
         itKeyListener->second->keyPressed(e);
   }
   return true;
}

bool InputTask::keyReleased(const OIS::KeyEvent &e)
{
   itKeyListener    = mKeyListeners.begin();
   itKeyListenerEnd = mKeyListeners.end();
   for(;itKeyListener != itKeyListenerEnd; ++itKeyListener)
   {
      if (this->isEnabled(itKeyListener->first))
         itKeyListener->second->keyReleased(e);
   }
   return true;
}

bool InputTask::mouseMoved(const OIS::MouseEvent &e)
{
   itMouseListener    = mMouseListeners.begin();
   itMouseListenerEnd = mMouseListeners.end();
   for(; itMouseListener != itMouseListenerEnd; ++itMouseListener)
   {
      if (this->isEnabled(itMouseListener->first))
         itMouseListener->second->mouseMoved(e);
   }
   return true;
}

bool InputTask::mousePressed(const OIS::MouseEvent &e,OIS::MouseButtonID id)
{
   itMouseListener    = mMouseListeners.begin();
   itMouseListenerEnd = mMouseListeners.end();
   for(; itMouseListener != itMouseListenerEnd; ++itMouseListener)
   {
      if (this->isEnabled(itMouseListener->first))
         itMouseListener->second->mousePressed(e, id);
   }
   return true;
}

bool InputTask::mouseReleased(const OIS::MouseEvent &e,OIS::MouseButtonID id)
{
   itMouseListener    = mMouseListeners.begin();
   itMouseListenerEnd = mMouseListeners.end();
   for(; itMouseListener != itMouseListenerEnd;++itMouseListener)
   {
      if (this->isEnabled(itMouseListener->first))
         itMouseListener->second->mouseReleased(e, id);
   }
   return true;
}

bool InputTask::povMoved(const OIS::JoyStickEvent &e, int pov)
{
   itJoystickListener    = mJoystickListeners.begin();
   itJoystickListenerEnd = mJoystickListeners.end();
   for(; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener)
   {
      if (this->isEnabled(itJoystickListener->first))
         itJoystickListener->second->povMoved(e, pov);
   }
   return true;
}

bool InputTask::axisMoved( const OIS::JoyStickEvent &e, int axis )
{
   itJoystickListener    = mJoystickListeners.begin();
   itJoystickListenerEnd = mJoystickListeners.end();
   for(; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener)
   {
      if (this->isEnabled(itJoystickListener->first))
         itJoystickListener->second->axisMoved(e, axis);
   }
   return true;
}

bool InputTask::sliderMoved( const OIS::JoyStickEvent &e, int sliderID )
{
   itJoystickListener    = mJoystickListeners.begin();
   itJoystickListenerEnd = mJoystickListeners.end();
   for(; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener)
   {
      if (this->isEnabled(itJoystickListener->first))
         itJoystickListener->second->sliderMoved(e, sliderID);
   }
   return true;
}

bool InputTask::buttonPressed(const OIS::JoyStickEvent &e, int button)
{
   itJoystickListener    = mJoystickListeners.begin();
   itJoystickListenerEnd = mJoystickListeners.end();
   for(; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener)
   {
      if (this->isEnabled(itJoystickListener->first))
         itJoystickListener->second->buttonPressed(e, button);
   }
   return true;
}

bool InputTask::buttonReleased(const OIS::JoyStickEvent &e, int button)
{
   itJoystickListener    = mJoystickListeners.begin();
   itJoystickListenerEnd = mJoystickListeners.end();
   for(; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener)
   {
      if (this->isEnabled(itJoystickListener->first))
         itJoystickListener->second->buttonReleased(e, button);
   }
   return true;
}


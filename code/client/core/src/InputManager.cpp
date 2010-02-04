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

#include "InputManager.h"

using namespace Client;

InputManager *InputManager::mInputManager;

InputManager::InputManager(void) : mInputSystem(0),mMouse(0),mKeyboard(0)
{ }

InputManager::~InputManager(void)
{
   if(mInputSystem)
   {
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

      /* Clear Listeners */
      mKeyListeners.clear();
      mMouseListeners.clear();
      mJoystickListeners.clear();
   }
}

void InputManager::initialise(Ogre::RenderWindow *renderWindow)
{
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
         renderWindow->getCustomAttribute( "WINDOW", &windowHnd );
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
      #if OIS_VERSION_MINOR == 2
      if(mInputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0)
      #else
      if(mInputSystem->numKeyboards() > 0)
      #endif
      {
         mKeyboard = static_cast<OIS::Keyboard*>(mInputSystem->createInputObject(OIS::OISKeyboard, true));
         mKeyboard->setEventCallback(this);
      }
	
      /* If possible create a buffered mouse */
      #if OIS_VERSION_MINOR == 2
      if(mInputSystem->getNumberOfDevices(OIS::OISMouse) > 0)
      #else
      if(mInputSystem->numMice() > 0)
      #endif
      {
         mMouse = static_cast<OIS::Mouse*>(mInputSystem->createInputObject(OIS::OISMouse, true));
         mMouse->setEventCallback(this);

         /* Get window size */
         unsigned int width, height, depth;
         int left, top;
         renderWindow->getMetrics( width, height, depth, left, top );

         /* Set mouse region */
         this->setWindowExtents(width, height);
      }

      /* If possible create all joysticks in buffered mode */
      #if OIS_VERSION_MINOR == 2
      if(mInputSystem->getNumberOfDevices(OIS::OISJoyStick) > 0)
      #else
      if(mInputSystem->numJoySticks() > 0)
      #endif
      {
         #if OIS_VERSION_MINOR == 2
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

void InputManager::capture( void )
{
   /* Need to capture / update each device every frame */
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

void InputManager::addKeyListener(OIS::KeyListener *keyListener, 
                                                const std::string& instanceName)
{
   if(mKeyboard)
   {
      /* Check for duplicate items */
      itKeyListener = mKeyListeners.find(instanceName);
      if(itKeyListener == mKeyListeners.end())
      {
         mKeyListeners[instanceName] = keyListener;
      }
   }
}

void InputManager::addMouseListener(OIS::MouseListener *mouseListener,
                                                const std::string& instanceName)
{
   if(mMouse)
   {
      /* Check for duplicate items */
      itMouseListener = mMouseListeners.find(instanceName);
      if( itMouseListener == mMouseListeners.end() )
      {
         mMouseListeners[instanceName] = mouseListener;
      }
   }
}

void InputManager::addJoystickListener(OIS::JoyStickListener *joystickListener,
                                                const std::string& instanceName)
{
   if(mJoysticks.size() > 0)
   {
      /* Check for duplicate items */
      itJoystickListener = mJoystickListeners.find(instanceName);
      if(itJoystickListener != mJoystickListeners.end())
      {
         mJoystickListeners[ instanceName ] = joystickListener;
      }
   }
}

void InputManager::removeKeyListener(const std::string& instanceName)
{
   itKeyListener = mKeyListeners.find(instanceName);
   if(itKeyListener != mKeyListeners.end())
   {
      mKeyListeners.erase(itKeyListener);
   }
}

void InputManager::removeMouseListener(const std::string& instanceName)
{
   itMouseListener = mMouseListeners.find(instanceName);
   if(itMouseListener != mMouseListeners.end())
   {
      mMouseListeners.erase(itMouseListener);
   }
}

void InputManager::removeJoystickListener(const std::string& instanceName)
{
   itJoystickListener = mJoystickListeners.find(instanceName);
   if(itJoystickListener != mJoystickListeners.end())
   {
      mJoystickListeners.erase(itJoystickListener);
   }
}

void InputManager::removeKeyListener(OIS::KeyListener *keyListener)
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

void InputManager::removeMouseListener(OIS::MouseListener *mouseListener)
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

void InputManager::removeJoystickListener(OIS::JoyStickListener *joystickListener)
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

void InputManager::removeAllListeners(void)
{
   mKeyListeners.clear();
   mMouseListeners.clear();
   mJoystickListeners.clear();
}

void InputManager::removeAllKeyListeners(void)
{
   mKeyListeners.clear();
}

void InputManager::removeAllMouseListeners(void)
{
   mMouseListeners.clear();
}

void InputManager::removeAllJoystickListeners(void)
{
   mJoystickListeners.clear();
}

void InputManager::setWindowExtents(int width, int height)
{
   /* 
    * Set mouse region (if window resizes, we should alter this to reflect as 
    * well)
    */
   const OIS::MouseState &mouseState = mMouse->getMouseState();
   mouseState.width  = width;
   mouseState.height = height;
}

OIS::Mouse* InputManager::getMouse(void)
{
   return mMouse;
}

OIS::Keyboard* InputManager::getKeyboard(void)
{
   return mKeyboard;
}

OIS::JoyStick* InputManager::getJoystick(unsigned int index)
{
   /* Make sure it's a valid index */
   if(index < mJoysticks.size())
   {
     return mJoysticks[index];
   }
   return 0;
}

int InputManager::getNumOfJoysticks(void)
{
   /* Cast to keep compiler happy */
   return (int) mJoysticks.size();
}

bool InputManager::keyPressed(const OIS::KeyEvent &e)
{
   itKeyListener    = mKeyListeners.begin();
   itKeyListenerEnd = mKeyListeners.end();
   for(;itKeyListener != itKeyListenerEnd; ++itKeyListener)
   {
      itKeyListener->second->keyPressed(e);
   }
   return true;
}

bool InputManager::keyReleased(const OIS::KeyEvent &e)
{
   itKeyListener    = mKeyListeners.begin();
   itKeyListenerEnd = mKeyListeners.end();
   for(; itKeyListener != itKeyListenerEnd; ++itKeyListener)
   {
      itKeyListener->second->keyReleased(e);
   }
   return true;
}

bool InputManager::mouseMoved(const OIS::MouseEvent &e)
{
   itMouseListener    = mMouseListeners.begin();
   itMouseListenerEnd = mMouseListeners.end();
   for(; itMouseListener != itMouseListenerEnd; ++itMouseListener)
   {
      itMouseListener->second->mouseMoved(e);
   }
   return true;
}

bool InputManager::mousePressed(const OIS::MouseEvent &e,OIS::MouseButtonID id)
{
   itMouseListener    = mMouseListeners.begin();
   itMouseListenerEnd = mMouseListeners.end();
   for(; itMouseListener != itMouseListenerEnd; ++itMouseListener)
   {
      itMouseListener->second->mousePressed(e, id);
   }
   return true;
}

bool InputManager::mouseReleased(const OIS::MouseEvent &e,OIS::MouseButtonID id)
{
   itMouseListener    = mMouseListeners.begin();
   itMouseListenerEnd = mMouseListeners.end();
   for(; itMouseListener != itMouseListenerEnd;++itMouseListener)
   {
      itMouseListener->second->mouseReleased(e, id);
   }
   return true;
}

bool InputManager::povMoved(const OIS::JoyStickEvent &e, int pov)
{
   itJoystickListener    = mJoystickListeners.begin();
   itJoystickListenerEnd = mJoystickListeners.end();
   for(; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener)
   {
      itJoystickListener->second->povMoved(e, pov);
   }
   return true;
}

bool InputManager::axisMoved( const OIS::JoyStickEvent &e, int axis )
{
   itJoystickListener    = mJoystickListeners.begin();
   itJoystickListenerEnd = mJoystickListeners.end();
   for(; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener)
   {
      itJoystickListener->second->axisMoved(e, axis);
   }
   return true;
}

bool InputManager::sliderMoved( const OIS::JoyStickEvent &e, int sliderID )
{
   itJoystickListener    = mJoystickListeners.begin();
   itJoystickListenerEnd = mJoystickListeners.end();
   for(; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener)
   {
      itJoystickListener->second->sliderMoved(e, sliderID);
   }
   return true;
}

bool InputManager::buttonPressed(const OIS::JoyStickEvent &e, int button)
{
   itJoystickListener    = mJoystickListeners.begin();
   itJoystickListenerEnd = mJoystickListeners.end();
   for(; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener)
   {
      itJoystickListener->second->buttonPressed(e, button);
   }
   return true;
}

bool InputManager::buttonReleased(const OIS::JoyStickEvent &e, int button)
{
   itJoystickListener    = mJoystickListeners.begin();
   itJoystickListenerEnd = mJoystickListeners.end();
   for(; itJoystickListener != itJoystickListenerEnd; ++itJoystickListener)
   {
      itJoystickListener->second->buttonReleased(e, button);
   }
   return true;
}

InputManager* InputManager::getSingletonPtr(void)
{
   if(!mInputManager)
   {
      mInputManager = new InputManager();
   }
   return mInputManager;
}


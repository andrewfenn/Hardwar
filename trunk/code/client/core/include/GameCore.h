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

#ifndef __HW_GameCore_H_
#define __HW_GameCore_H_

#include <Ogre.h>
#include <libintl.h>
#include <iostream>
#include <boost/filesystem.hpp>

#include "Network.h"
#include "Console.h"
#include "md5.h"

namespace Client
{
   class GameCore
   {
      public:
         GameCore();
            /** This method is used by Client::Console to toggle the debug window that
            shows FPS information.
            @param key
                   The command name typed in the console.
            @param value
                   A boolean value that either shows or hides the window.
            */
         void cmd_showFPS(const Ogre::UTFString&, const Ogre::UTFString&);
            /** This method is used by Client::Console to toggle the debug window that
            shows network information.
            @param key
                   The command name typed in the console.
            @param value
                   A boolean value that either shows or hides the window.
            */
         void cmd_showNet(const Ogre::UTFString&, const Ogre::UTFString&);
            /** This method is used by Client::Console to change the FPS limit.
            @remarks
                   The value can not be below 25 FPS.
            @param key
                   The command name typed in the console.
            @param value
                   An integer for the new FPS limit.
            */
         void cmd_maxFPS(const Ogre::UTFString&, const Ogre::UTFString&);
            /** This method is used by Client::Console to show bounding boxes on
                all 3d objects for debug purposes.
            @param key
                   The command name typed in the console.
            @param value
                   A boolean value that either shows or hides the boxes.
            */
         void cmd_showBoundingBox(const Ogre::UTFString&, const Ogre::UTFString&);
            /** This method is used by Client::Console to remotely log into the server.
            @param key
                   The command name typed in the console.
            @param value
                   The password typed into the console.
            */
         void cmd_screenshot(const Ogre::UTFString&, const Ogre::UTFString&);
            /** This method is used by Client::Console to take a screenshot
            @remarks
                   The filename value is optional.
            @param key
                   The command name typed in the console.
            @param value
                   A string value for the filename.
            */
         void cmd_remoteConnect(const Ogre::UTFString&, const Ogre::UTFString&);
            /** This method returns the delay time between render loops depending
                upon what the max FPS limit is set to. */
         unsigned short getDelayTime(void);
         void update(unsigned long);
      private:
         Console *mConsole;
         void showFPS(void);
         void showNet(void);
         bool mShowFPS;
         bool mShowNet;
         unsigned short mMaxFPS;
         unsigned short mWaitTime;
         unsigned long mFPStimer;
   };
}

#endif /* __HW_GameCore_H_ */

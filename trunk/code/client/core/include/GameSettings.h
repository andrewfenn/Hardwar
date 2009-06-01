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

#ifndef __GAMESETTINGS_H_
#define __GAMESETTINGS_H_

#include <OgreRoot.h>
#include <map>
#include <libintl.h>

#include "Console.h"
#include "Network.h"
#include "md5.h"

namespace Client
{

/** The GameSettings class
        @remarks
            The Client::GameSettings class is where operation variables are
        stored, changed, loaded and saved. It also includes functions used by
        Client::Console which change the variables stored here.
    */
class GameSettings
{
   public:
      ~GameSettings(void);
      static GameSettings* getSingletonPtr(void);

      void update(unsigned long);
      /** This method is used by Client::Console to toggle the debug window that
      shows FPS information.
      @param key
             The command name typed in the console.
      @param value
             A boolean value that either shows or hides the window.
      command.
      */
      void cmd_showFPS(const Ogre::UTFString&, const Ogre::UTFString&);
      /** This method is used by Client::Console to change the FPS limit.
      @remarks
             The value can not be below 25 FPS.
      @param key
             The command name typed in the console.
      @param value
             An integer for the new FPS limit.
      command.
      */
      void cmd_maxFPS(const Ogre::UTFString&, const Ogre::UTFString&);
      /** This method is used by Client::Console to toggle the debug window that
      shows network information.
      @param key
             The command name typed in the console.
      @param value
             A boolean value that either shows or hides the window.
      command.
      */
      void cmd_showNet(const Ogre::UTFString&, const Ogre::UTFString&);
      /** This method is used by Client::Console to remotely log into the server.
      @param key
             The command name typed in the console.
      @param value
             The password typed into the console.
      command.
      */
      void cmd_remoteConnect(const Ogre::UTFString&, const Ogre::UTFString&);
      /** This method returns the delay time between render loops depending
      upon what the max FPS limit is set to. */
      unsigned short getDelayTime(void);
      /** Creates or changes a value stored in mGameSettings .
      @remarks
             All values are stored as Strings. You can use Ogre::StringConverter
      to convert between types.
      @param name
             The name of the value trying to be created or changed
      @param value
             The value to be stored
      */
      void setOption(const Ogre::UTFString, Ogre::UTFString);
      /** Retrieves a value stored in mGameSettings 
      @remarks
             If there is no value found then an empty Ogre::String is returned.
      @param name
             The name of the value trying to be retrieved
      */
      Ogre::UTFString getOption(const Ogre::UTFString);

   private:      Console *mConsole;
      void showFPS(void);
      void showNet(void);

      typedef std::map<Ogre::UTFString,Ogre::UTFString> Option;
      Option mOptions;

      static GameSettings *mGameSettings;
      GameSettings(void);
      GameSettings(const GameSettings&) { }
      GameSettings & operator = (const GameSettings&);

      bool mShowFPS;
      bool mShowNet;
      unsigned short mMaxFPS;
      unsigned short mWaitTime;
      unsigned long mFPStimer;      
};

}
#endif /* __GAMESETTINGS_H_ */


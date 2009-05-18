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

#include "Console.h"
#include "Network.h"
#include "md5.h"

namespace Client
{

class GameSettings
{
   public:
      ~GameSettings(void);
      static GameSettings* getSingletonPtr(void);

      void update(unsigned long);
      void cmd_showFPS(const Ogre::UTFString&, const Ogre::UTFString&);
      void cmd_maxFPS(const Ogre::UTFString&, const Ogre::UTFString&);
      void cmd_showNet(const Ogre::UTFString&, const Ogre::UTFString&);
      void cmd_remoteConnect(const Ogre::UTFString&, const Ogre::UTFString&);
      unsigned short getDelayTime(void);
      void setOption(const Ogre::UTFString, Ogre::UTFString);
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


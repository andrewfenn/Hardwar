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

#include "GameSettings.h"

GameSettings* GameSettings::mGameSettings;

GameSettings::GameSettings(void)
{
   Ogre::ConfigFile lconfig;
   lconfig.load("settings.cfg");

   setOption("NetworkRetryLimit", lconfig.getSetting("RetryLimit", "Network", "5"));
   setOption("NetworkTimeout", lconfig.getSetting("Timeout", "Network", "5"));
}

GameSettings::~GameSettings(void)
{

}

void GameSettings::setOption(const Ogre::UTFString lName, Ogre::UTFString lValue)
{
   mOptions[lName] = lValue;
}

Ogre::UTFString GameSettings::getOption(const Ogre::UTFString lName)
{
   Option::iterator iter = mOptions.find(lName);
   if (iter != mOptions.end())
   {
      return mOptions[lName];
   }
   return Ogre::UTFString("");
}

GameSettings* GameSettings::getSingletonPtr(void)
{
   if(!mGameSettings)
   {
      mGameSettings = new GameSettings();
   }

   return mGameSettings;
}

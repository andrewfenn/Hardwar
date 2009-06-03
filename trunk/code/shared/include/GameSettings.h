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

#ifndef __HW_GAMESETTINGS_H_
#define __HW_GAMESETTINGS_H_

#include <map>

#include "Ogre.h"
#include "Config.h"


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
   private:
      typedef std::map<Ogre::UTFString,Ogre::UTFString> Option;
      Option mOptions;

      static GameSettings *mGameSettings;
      GameSettings(void);
      GameSettings(const GameSettings&) { }
      GameSettings & operator = (const GameSettings&);
};

#endif /* __HW_GAMESETTINGS_H_ */


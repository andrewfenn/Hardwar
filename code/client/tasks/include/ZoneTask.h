/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2010 - 2012  Andrew Fenn
    
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

#pragma once

#include <Ogre.h>
#include <boost/filesystem.hpp>

#include "Zone.h"
#include "GameTask.h"

namespace Client
{
typedef std::vector<Zone> Zones;

/** The Zone Manager
        @remarks
            The Client::ZoneManager class is our root class which deals with the
            initialisation and running of our Zone.
    */
class ZoneTask : public GameTask
{
   public:
      void changeSize(Ogre::RenderWindow*) {}
      void changeFocus(Ogre::RenderWindow*) {}
      /** Pre-load all game assets related to the levels */
      void init();
      /** Creates a new zone */
      void create(const int, const Ogre::String);
      /** Clears all currently loaded zones */
      void clear();
      /** Returns the total number of different zones */
      int getTotal();
      /** Retrieves a pointer to the current zone that the player is in. */
      Zone* getCurrent();
      void shutdown();
      void update();
   private:
      Zones mZones;
};

}


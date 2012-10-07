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
#include "OgreMaxScene.hpp"

#include "Building.h"

namespace Client
{

/** A zone is an outside player area. */
class Zone
{
   public:
      Zone(const Ogre::String);
      ~Zone();
      bool addBuilding(Hardwar::Building);
      Hardwar::Building getBuildingByName(const Ogre::String);
   private:
      std::vector<Hardwar::Building> mBuildings;

      /** Create a building specified from the building list */
      bool drawBuilding(Hardwar::Building);
};

}


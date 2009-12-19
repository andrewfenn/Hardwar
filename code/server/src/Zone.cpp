/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright (C) 2009  Andrew Fenn
    
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

#include "Zone.h"

using namespace Server;

Zone::Zone()
{
}

Zone::~Zone()
{
}

Building::iterator Zone::getBuildings(void)
{
   return mBuildings.begin();
}

unsigned int Zone::numBuildings(void)
{
   return mBuildings.size();
}

bool Zone::addBuilding(const unsigned int id, Hardwar::Building building)
{
   mBuildings.insert(std::pair<unsigned int,Hardwar::Building>(id, building));

   Ogre::Vector3 pos, rot;
   Ogre::String mesh;
   pos = building.getPosition();
   rot = building.getRotation();
   mesh = building.getMeshName();
   /* FIXME: Remove the printf after debugging */
   printf("New Building - Position: %s - Rotation: %s - Mesh: %s\n",
                              Ogre::StringConverter::toString(pos).c_str(),
                              Ogre::StringConverter::toString(rot).c_str(),
                              mesh.c_str());
   return true;
}

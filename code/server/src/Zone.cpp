/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2009  Andrew Fenn
    
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

std::vector<Hardwar::Building> Zone::getBuildings(void)
{
   return mBuildings;
}

unsigned int Zone::numBuildings(void)
{
   return mBuildings.size();
}

bool Zone::addBuilding(Hardwar::Building building)
{
   mBuildings.push_back(building);

   Ogre::Vector3 pos;
   Ogre::Quaternion rot;
   Ogre::String mesh;
   pos = building.getPosition();
   rot = building.getRotation();
   mesh = building.getMeshName();
   /* FIXME: Remove the printf after debugging */
   printf("Zone: New Building - ID: %d - Position: %s - Rotation: %s - Mesh: %s\n",
                              building.getID(),
                              Ogre::StringConverter::toString(pos).c_str(),
                              Ogre::StringConverter::toString(rot).c_str(),
                              mesh.c_str());
   return true;
}

bool Zone::editBuilding(Hardwar::Building building)
{
   for (std::vector<Hardwar::Building>::iterator buildItr=mBuildings.begin(); buildItr != mBuildings.end(); buildItr++)
   {
      if (building.getID() == buildItr->getID())
      {
         mBuildings.erase(buildItr);
         mBuildings.push_back(building);
         Ogre::Vector3 pos;
         Ogre::Quaternion rot;
         Ogre::String mesh;
         pos = building.getPosition();
         rot = building.getRotation();
         mesh = building.getMeshName();
         /* FIXME: Remove the printf after debugging */
         printf("Zone: Edit Building - Position: %s - Rotation: %s - Mesh: %s\n",
                                    Ogre::StringConverter::toString(pos).c_str(),
                                    Ogre::StringConverter::toString(rot).c_str(),
                                    mesh.c_str());
         return true;
      }
   }
   return false;
}

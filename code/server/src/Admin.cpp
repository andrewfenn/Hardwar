/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright (C) 2008-2009  Andrew Fenn
    
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

#include "Admin.h"

using namespace Server;

Admin::Admin()
{

}

Admin::~Admin()
{

}

void Admin::processRequest(dataPacket lPacket)
{
   switch(lPacket.getMessage())
   {
      case add_building:
         {
            Hardwar::Building building;
            building.unserialize(lPacket);
            addBuilding(building);
         }
      break;
      default:
         std::cout << gettext("Undefined Admin Message: ") << lPacket.getMessage() << std::endl;
      break;
   }
}

Hardwar::Buildings Admin::getBuildings()
{
   Hardwar::Buildings list = mBuildings;
   mBuildings.clear();
   return list;
}

void Admin::addBuilding(Hardwar::Building building)
{
   mBuildings.insert(std::pair<unsigned int,Hardwar::Building>(0, building));
}

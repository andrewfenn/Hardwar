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

#include "Admin.h"

using namespace Server;

Admin::Admin()
{
   mWantsWorldSave = false;
}

Admin::~Admin()
{

}

void Admin::processRequest(dataPacket lPacket)
{
   switch(lPacket.getMessage())
   {
      case add_building:
            addBuilding(lPacket);
      break;
      case edit_building:
            editBuilding(lPacket);
      break;
      case save_world_data:
         requestSave();
      break;
      default:
         std::cout << gettext("Undefined Admin Message: ") << lPacket.getMessage() << std::endl;
      break;
   }
}

Hardwar::Buildings Admin::getAddBuildings()
{
   Hardwar::Buildings list = mAddBuildings;
   mAddBuildings.clear();
   return list;
}

Hardwar::Buildings Admin::getEditBuildings()
{
   Hardwar::Buildings list = mEditBuildings;
   mEditBuildings.clear();
   return list;
}

void Admin::addBuilding(dataPacket lPacket)
{
   Hardwar::Building building;
   building.unserialize(lPacket);
   mAddBuildings.insert(std::pair<unsigned int,Hardwar::Building>(0, building));
}

void Admin::editBuilding(dataPacket lPacket)
{
   Hardwar::Building building;
   building.unserialize(lPacket);
   mEditBuildings.insert(std::pair<unsigned int,Hardwar::Building>(0, building));
}

bool Admin::getWorldSaveReq()
{
   bool wantsave = mWantsWorldSave;
   mWantsWorldSave = false;
   return wantsave;
}

void Admin::requestSave()
{
   mWantsWorldSave = true;   
}

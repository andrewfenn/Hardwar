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
            HWBuilding building;
            std::memcpy(&building, lPacket.getContents(), sizeof(HWBuilding));
            LevelManager::getSingletonPtr()->addBuilding((unsigned int)0, building);
         }
      break;
      default:
         std::cout << "PacketMessage: " << lPacket.getMessage() << " data: " << lPacket.getContents() << std::endl;
      break;
   }
}

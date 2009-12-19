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

#ifndef __ADMIN_H_
#define __ADMIN_H_

#include "enet/enet.h"
#include <map>
#include <Ogre.h>

#include "srvstructs.h"
#include "DataPacket.h"
#include "Building.h"

namespace Server
{
   typedef std::multimap<enet_uint8,ENetEvent> Message;

   /** Admin
           @remarks
               Processes requests made by an admin
       */
   class Admin
   {
      public:
         Admin();
         ~Admin();
         void processRequest(dataPacket lPacket);
         
   };
}
#endif /* __ADMIN_H_ */


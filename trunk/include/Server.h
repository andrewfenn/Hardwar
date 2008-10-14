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

#ifndef Server_H
#define Server_H

#include "WorldManager.h"
extern "C" {
	#include <enet/enet.h>
}

class Server {
   public:
      Server();
      ~Server();
      
   private:
      WorldManager *mWorldMgr;// World manager, manages the craters build placement, etc
      void setupServer();
      void serverLoop();
      ENetHost * server;
      ENetAddress address;
};
#endif


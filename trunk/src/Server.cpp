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

#include "Server.h"


Server::Server() {
   // Start the world manager and load up a world
   mWorldMgr = new WorldManager;
   // load up our SQLite world database
//   if (!mWorldMgr->loadWorldData("world/default.db", mSceneMgr))
  //    exit(1);
    /*if (enet_initialize() != 0)
    {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
    }*/
}

Server::~Server() {
   // enet_deinitialize();
}

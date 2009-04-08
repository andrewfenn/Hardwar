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

#ifndef __SERVER_H_
#define __SERVER_H_

#include <Ogre.h>
#include <string>
#include <libintl.h>

#include "hwstructs.h"
#include "WorldManager.h"
#include "FileManager.h"
#include "srvstructs.h"
#include "enet/enet.h"

namespace Server
{

class ServerMain
{
   public:
      ServerMain();
      ServerMain(Ogre::ConfigFile);
      ~ServerMain();
   private:
      ENetHost            *mServer;
      typedef std::map<enet_uint16, Player> Clients;
      Clients             mPlayer;
      unsigned int        mPlayerCount;
      Ogre::ConfigFile    mConfig;

//      WorldManager        *mWorldMgr;

      bool    setupServer(int, std::string);
      bool    setupGame();
      void    serverLoop();
      void    clientLoop();
      bool    message(ENetPeer*,const void*, size_t, enet_uint8, enet_uint32);
};

}
#endif /* __SERVER_H_ */


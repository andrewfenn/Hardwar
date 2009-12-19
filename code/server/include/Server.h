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
#include "enet/enet.h"

#include "srvstructs.h"
#include "ClientManager.h"
#include "GameManager.h"

namespace Server
{
   /** The Main Server Loop
           @remarks
               The Server::ServerMain class acts as a hub where messages being
               received by Enet are relayed on to the different threads to be
               processed.
    */
   class ServerMain
   {
      public:
         ServerMain(Ogre::ConfigFile);
         ~ServerMain();

         bool    setup(int, std::string);
         bool    setupGame();
         void    serverLoop();
         ClientManager* mClientMgr;
         GameManager* mGameMgr;
      private:
         ENetHost            *mServer;
         unsigned int        mPlayerCount;
         Ogre::ConfigFile    mConfig;
   };
}
#endif /* __SERVER_H_ */

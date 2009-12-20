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

#ifndef __CLIENT_MANAGER_H_
#define __CLIENT_MANAGER_H_

#include "Client.h"
#include "enet/enet.h"
#include <libintl.h>

namespace Server
{
   class Client;
   
   class ClientManager
   {
      public:

         void clientLoop(void);
         /** Add a new player to the client list. */
         void add(ENetPeer*);
         /** Get a player currently connected */
         Client* get(ENetPeer*);
         /** Remove a player for the list */
         void remove(ENetPeer*);
         /** Add a message in the client's event list */
         void message(const ENetEvent);
         /** Sets the enet host for server communication */
         void setHost(ENetHost*);
         /** send a packet of data to a specific client or set the peer to 0 for broadcasting */
         bool send(dataPacket, enet_uint8, enet_uint32, ENetPeer* peer = 0);

         ~ClientManager();
         ClientManager();
      private:
         typedef std::map<enet_uint16, Server::Client*> Clients;
         Clients mPlayers;
         ENetHost * mHost;
   };
}

#endif /* __CLIENT_MANAGER_H_ */

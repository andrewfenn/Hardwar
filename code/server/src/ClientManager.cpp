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

#include "ClientManager.h"

using namespace Server;

ClientManager::~ClientManager(void)
{
}

ClientManager::ClientManager(void)
{
}

void ClientManager::add(ENetPeer *lpeer)
{
   mPlayers[lpeer->incomingPeerID] = new Client();
   mPlayers[lpeer->incomingPeerID]->setPeer(lpeer);
   /* start new client thread */
   mPlayers[lpeer->incomingPeerID]->addThread();
   printf(gettext("Thread: Client - %d - started\n"), lpeer->incomingPeerID);
}

Client* ClientManager::get(ENetPeer *lpeer)
{
   if (mPlayers[lpeer->incomingPeerID] != 0)
   {
      return mPlayers[lpeer->incomingPeerID];
   }
   return 0;
}

void ClientManager::remove(ENetPeer *lpeer)
{
   mPlayers[lpeer->incomingPeerID]->removeThread();
   delete mPlayers[lpeer->incomingPeerID];
   mPlayers[lpeer->incomingPeerID] = 0;
   mPlayers.erase(lpeer->incomingPeerID);
}

void ClientManager::message(const ENetEvent lEvent)
{
   if (mPlayers[lEvent.peer->incomingPeerID] != 0)
   {
      mPlayers[lEvent.peer->incomingPeerID]->addMessage(lEvent);
   }
}

void ClientManager::setHost(ENetHost* lHost)
{
   mHost = lHost;
}

Clients* ClientManager::list()
{
   return &mPlayers;
}

bool ClientManager::send(dataPacket data, enet_uint8 channel, enet_uint32 priority, ENetPeer *peer)
{
   bool result = true;
   ENetPacket * packet = enet_packet_create(data.getContents(), data.size(), priority);

   if (peer == 0)
   {
      enet_host_broadcast(mHost, channel, packet);
   }
   else
   {
      if (enet_peer_send(peer, channel, packet) <0)
      {
         result = false;
      }
   }
   enet_host_flush(mHost);
   return result;
}

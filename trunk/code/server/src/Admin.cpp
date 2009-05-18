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
   mRunThread = true;
}

Admin::~Admin()
{
   mRunThread = false;
}

void Admin::addMessage(const ENetEvent lEvent)
{
   mMessages.insert(std::pair<enet_uint8,ENetEvent>(lEvent.channelID, lEvent));
}

void Admin::makeThread(void)
{
   /* creates a new thread */
   boost::thread mThread(boost::bind(&Admin::loop, this));
   return;
}

void Admin::stopThread(void)
{
   mRunThread = false;
}

void Admin::loop(void)
{
   Message::iterator itEvent;
   Message lMessages;
   while(mRunThread)
   {
      if (mMessages.size() > 0)
      {
         /* TODO: mutex lock before copying and deleting */
         lMessages = mMessages;
         mMessages.clear();

         for (itEvent=lMessages.begin(); itEvent != lMessages.end(); itEvent++)
         {
            printf ("len:%u - value:%s - client:%d - channel %u.\n",
                                (intptr_t) (*itEvent).second.packet->dataLength,
                                         (char*) (*itEvent).second.packet->data,
                                         (*itEvent).second.peer->incomingPeerID,
                                                              (*itEvent).first);

            if (strcmp((char*)(*itEvent).second.packet->data, "login") == 0)
            {
               enet_packet_destroy((*itEvent).second.packet);
               itEvent++;
               printf ("Hash is:%s\n", (char*) (*itEvent).second.packet->data);
               /* FIXME: There is no password */
               sendMessage((*itEvent).second.peer, "login", sizeof("login")+1, SERVER_CHANNEL_ADMIN, ENET_PACKET_FLAG_RELIABLE);
               sendMessage((*itEvent).second.peer, "1", 2, SERVER_CHANNEL_ADMIN, ENET_PACKET_FLAG_RELIABLE);
            }
            
            /* finished with the packet, destory it */
            enet_packet_destroy((*itEvent).second.packet);
         }
      }
   }
}

bool Admin::sendMessage(ENetPeer *peer,const void* msg, size_t size, 
                                       enet_uint8 channel, enet_uint32 priority)
{
   bool result = true;
   ENetPacket * packet = enet_packet_create (msg, size, priority);

   if (enet_peer_send(peer, channel, packet) <0)
   {
      result = false;
   }
   return result;
}
